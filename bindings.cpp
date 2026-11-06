#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>
#include <pybind11/functional.h>
#include "cognitive_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(cognitive_engine, m) {
    m.doc() = "ACR Cognitive Engine - C++ core for artificial cognitive research";
    
    // SignalType enum
    py::enum_<acr::SignalType>(m, "SignalType")
        .value("NOVELTY", acr::SignalType::NOVELTY)
        .value("UNCERTAINTY", acr::SignalType::UNCERTAINTY)
        .value("PREDICTION_ERROR", acr::SignalType::PREDICTION_ERROR)
        .value("GOAL_RELEVANCE", acr::SignalType::GOAL_RELEVANCE)
        .value("CONTRADICTION", acr::SignalType::CONTRADICTION)
        .value("INFORMATION_GAIN", acr::SignalType::INFORMATION_GAIN)
        .export_values();
    
    // MotivationSignal
    py::class_<acr::MotivationSignal>(m, "MotivationSignal")
        .def(py::init<>())
        .def_readwrite("type", &acr::MotivationSignal::type)
        .def_readwrite("value", &acr::MotivationSignal::value)
        .def_readwrite("weight", &acr::MotivationSignal::weight)
        .def_readwrite("source_module", &acr::MotivationSignal::source_module)
        .def("weighted_value", &acr::MotivationSignal::weighted_value);
    
    // CognitiveItem
    py::class_<acr::CognitiveItem>(m, "CognitiveItem")
        .def(py::init<>())
        .def_readonly("id", &acr::CognitiveItem::id)
        .def_readonly("content", &acr::CognitiveItem::content)
        .def_readonly("source_module", &acr::CognitiveItem::source_module)
        .def_readonly("priority_score", &acr::CognitiveItem::priority_score)
        .def_readonly("timestamp", &acr::CognitiveItem::timestamp)
        .def_readonly("signals", &acr::CognitiveItem::signals);
    
    // SelfModelState
    py::class_<acr::SelfModelState>(m, "SelfModelState")
        .def(py::init<>())
        .def_readonly("runtime_status", &acr::SelfModelState::runtime_status)
        .def_readonly("current_focus", &acr::SelfModelState::current_focus)
        .def_readonly("active_goals", &acr::SelfModelState::active_goals)
        .def_readonly("confidence_level", &acr::SelfModelState::confidence_level)
        .def_readonly("version", &acr::SelfModelState::version)
        .def_readonly("last_updated", &acr::SelfModelState::last_updated);
    
    // AuditRecord
    py::class_<acr::AuditRecord>(m, "AuditRecord")
        .def(py::init<>())
        .def_readonly("sequence_id", &acr::AuditRecord::sequence_id)
        .def_readonly("timestamp", &acr::AuditRecord::timestamp)
        .def_readonly("module", &acr::AuditRecord::module)
        .def_readonly("action", &acr::AuditRecord::action)
        .def_readonly("details", &acr::AuditRecord::details);
    
    // CognitiveEngine facade
    py::class_<acr::CognitiveEngine>(m, "CognitiveEngine")
        .def(py::init<>())
        .def("initialize", &acr::CognitiveEngine::initialize,
             py::arg("runtime_version"),
             "Initialize engine and set self-model to RUNNING state")
        .def("submit_to_workspace", &acr::CognitiveEngine::submit_to_workspace,
             py::arg("id"), py::arg("content"), py::arg("source"), py::arg("signals"),
             "Submit candidate information with motivation signals")
        .def("process_next", &acr::CognitiveEngine::process_next,
             "Select and remove highest-priority item from workspace")
        .def("get_self_model", &acr::CognitiveEngine::get_self_model,
             "Get thread-safe snapshot of current self-model state")
        .def("update_self_model", &acr::CognitiveEngine::update_self_model,
             py::arg("focus"), py::arg("confidence"), py::arg("reason"),
             "Atomically update self-model focus and confidence")
        .def("get_audit_trail", &acr::CognitiveEngine::get_audit_trail,
             py::arg("count") = 100,
             "Retrieve recent audit records")
        .def("workspace_pending", &acr::CognitiveEngine::workspace_pending,
             "Number of items currently in workspace queue");
}
