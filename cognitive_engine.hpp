#pragma once

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <chrono>
#include <functional>
#include <optional>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <sstream>

namespace acr {

// ============================================================================
// Core Types & Signals (Section 4.6, 4.10)
// ============================================================================

enum class SignalType : uint8_t {
    NOVELTY,
    UNCERTAINTY,
    PREDICTION_ERROR,
    GOAL_RELEVANCE,
    CONTRADICTION,
    INFORMATION_GAIN
};

struct MotivationSignal {
    SignalType type;
    double value;      // Normalized [0.0, 1.0]
    double weight;     // Configurable importance
    std::string source_module;
    
    double weighted_value() const noexcept { return value * weight; }
};

struct CognitiveItem {
    uint64_t id;
    std::string content;
    std::string source_module;
    double priority_score;
    std::chrono::steady_clock::time_point timestamp;
    std::vector<MotivationSignal> signals;
    
    // For priority queue ordering
    bool operator<(const CognitiveItem& other) const noexcept {
        return priority_score < other.priority_score;
    }
};

// ============================================================================
// Audit & Traceability (Section 2.6, 8)
// ============================================================================

struct AuditRecord {
    uint64_t sequence_id;
    std::chrono::system_clock::time_point timestamp;
    std::string module;
    std::string action;
    std::string details;
};

class AuditLog {
public:
    void record(const std::string& module, const std::string& action, 
                const std::string& details = "") {
        std::lock_guard<std::mutex> lock(mutex_);
        records_.push_back({next_seq_++, std::chrono::system_clock::now(),
                           module, action, details});
        // In production: bounded buffer or async flush to persistence layer
    }
    
    std::vector<AuditRecord> get_recent(size_t count) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto start = records_.size() > count ? records_.end() - count : records_.begin();
        return {start, records_.end()};
    }

private:
    mutable std::mutex mutex_;
    std::vector<AuditRecord> records_;
    std::atomic<uint64_t> next_seq_{1};
};

// ============================================================================
// Self-Model (Section 4.3, Level A)
// ============================================================================

struct SelfModelState {
    std::string runtime_status;
    std::string current_focus;
    std::vector<std::string> active_goals;
    double confidence_level;
    uint64_t version;
    std::chrono::system_clock::time_point last_updated;
};

class SelfModel {
public:
    SelfModelState get_snapshot() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return state_;
    }
    
    bool update(std::function<void(SelfModelState&)> modifier, 
                const std::string& reason, AuditLog& audit) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        auto previous_version = state_.version;
        modifier(state_);
        state_.version++;
        state_.last_updated = std::chrono::system_clock::now();
        
        audit.record("SelfModel", "StateUpdate",
                     "v" + std::to_string(previous_version) + "->" + 
                     std::to_string(state_.version) + ": " + reason);
        return true;
    }

private:
    mutable std::shared_mutex mutex_;
    SelfModelState state_{"INITIALIZED", "", {}, 1.0, 0, 
                          std::chrono::system_clock::now()};
};

// ============================================================================
// Attention Module (Section 4.6)
// ============================================================================

class AttentionModule {
public:
    explicit AttentionModule(std::unordered_map<SignalType, double> weights = {})
        : weights_(std::move(weights)) {
        // Default weights if not provided
        if (weights_.empty()) {
            weights_[SignalType::NOVELTY] = 0.2;
            weights_[SignalType::UNCERTAINTY] = 0.25;
            weights_[SignalType::PREDICTION_ERROR] = 0.3;
            weights_[SignalType::GOAL_RELEVANCE] = 0.15;
            weights_[SignalType::CONTRADICTION] = 0.05;
            weights_[SignalType::INFORMATION_GAIN] = 0.05;
        }
    }
    
    double compute_priority(const std::vector<MotivationSignal>& signals) const noexcept {
        double score = 0.0;
        for (const auto& sig : signals) {
            auto it = weights_.find(sig.type);
            double w = (it != weights_.end()) ? it->second : 0.0;
            score += sig.value * w;
        }
        // Clamp to [0, 1]
        return std::max(0.0, std::min(1.0, score));
    }
    
    void set_weight(SignalType type, double weight) {
        weights_[type] = std::max(0.0, std::min(1.0, weight));
    }

private:
    std::unordered_map<SignalType, double> weights_;
};

// ============================================================================
// Global Workspace (Section 4.1)
// ============================================================================

class GlobalWorkspace {
public:
    struct Config {
        size_t max_capacity = 1024;
        std::chrono::milliseconds max_item_age{30000};
    };
    
    explicit GlobalWorkspace(Config config = {}, AuditLog* audit = nullptr)
        : config_(config), audit_(audit) {}
    
    /// Thread-safe submission of candidate information
    bool submit(CognitiveItem item) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (queue_.size() >= config_.max_capacity) {
            if (audit_) audit_->record("GlobalWorkspace", "Drop",
                "Capacity exceeded, dropping lowest priority item");
            // Priority queue doesn't support efficient removal of lowest;
            // in production use a bounded heap or skip-list
            return false;
        }
        
        queue_.push(std::move(item));
        
        if (audit_) {
            audit_->record("GlobalWorkspace", "Submit",
                "Item#" + std::to_string(queue_.top().id) + 
                " priority=" + std::to_string(queue_.top().priority_score));
        }
        return true;
    }
    
    /// Extract highest-priority item for broadcast/processing
    std::optional<CognitiveItem> select_top() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (queue_.empty()) return std::nullopt;
        
        // Evict stale items
        auto now = std::chrono::steady_clock::now();
        while (!queue_.empty() && 
               (now - queue_.top().timestamp) > config_.max_item_age) {
            if (audit_) audit_->record("GlobalWorkspace", "Evict",
                "Stale item#" + std::to_string(queue_.top().id));
            queue_.pop();
        }
        
        if (queue_.empty()) return std::nullopt;
        
        CognitiveItem top = queue_.top();
        queue_.pop();
        
        if (audit_) audit_->record("GlobalWorkspace", "Select",
            "Broadcasting item#" + std::to_string(top.id));
        
        return top;
    }
    
    size_t pending_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    Config config_;
    mutable std::mutex mutex_;
    std::priority_queue<CognitiveItem> queue_;
    AuditLog* audit_;
};

// ============================================================================
// CognitiveEngine Facade (Integration Point for pybind11)
// ============================================================================

class CognitiveEngine {
public:
    CognitiveEngine() 
        : workspace_(GlobalWorkspace::Config{}, &audit_),
          attention_() {}
    
    // --- Public API for Python binding ---
    
    void initialize(const std::string& runtime_version) {
        self_model_.update([&](SelfModelState& s) {
            s.runtime_status = "RUNNING";
            s.runtime_status += " v" + runtime_version;
        }, "Engine initialized", audit_);
    }
    
    bool submit_to_workspace(uint64_t id, const std::string& content,
                             const std::string& source,
                             const std::vector<MotivationSignal>& signals) {
        CognitiveItem item;
        item.id = id;
        item.content = content;
        item.source_module = source;
        item.signals = signals;
        item.timestamp = std::chrono::steady_clock::now();
        item.priority_score = attention_.compute_priority(signals);
        
        return workspace_.submit(std::move(item));
    }
    
    std::optional<CognitiveItem> process_next() {
        return workspace_.select_top();
    }
    
    SelfModelState get_self_model() const {
        return self_model_.get_snapshot();
    }
    
    bool update_self_model(const std::string& focus, double confidence,
                           const std::string& reason) {
        return self_model_.update([&](SelfModelState& s) {
            s.current_focus = focus;
            s.confidence_level = confidence;
        }, reason, audit_);
    }
    
    std::vector<AuditRecord> get_audit_trail(size_t count) const {
        return audit_.get_recent(count);
    }
    
    size_t workspace_pending() const {
        return workspace_.pending_count();
    }

private:
    AuditLog audit_;
    SelfModel self_model_;
    AttentionModule attention_;
    GlobalWorkspace workspace_;
};

} // namespace acr
