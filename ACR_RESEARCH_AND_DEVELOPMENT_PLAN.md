# Artificial Cognitive Research
## Research and Development Plan

**Project:** Artificial Cognitive Research (ACR)  
**Repository:** `EnumAEleeS/Artificial-Cognitive-Research`  
**Document status:** Foundational research and engineering plan  
**Implementation principle:** Build measurable functional capabilities while keeping claims about subjective consciousness explicitly separate from engineering results.

---

## 1. Research Objective

Artificial Cognitive Research (ACR) investigates architectures for artificial systems that exhibit increasingly sophisticated cognitive organization and bounded functional autonomy.

The central engineering question is:

> Can a software system maintain an evolving internal model, integrate information across specialized processes, evaluate its own uncertainty and performance, form and revise goals from internal state, and execute authorized actions over time without requiring a human to specify every individual step?

The project does **not** assume that implementing these capabilities creates subjective experience. It aims to develop an architecture and experimental framework through which functional properties can be implemented, measured, compared, and critically evaluated.

### 1.1 Distinctions that must remain explicit

The following concepts are related but not interchangeable:

- **Functional autonomy:** the ability to continue bounded processes, select actions, and manage tasks under defined permissions.
- **Goal formation:** generating candidate objectives from internal state, observations, prediction errors, unresolved questions, and other computational signals.
- **Persistent self-model:** maintaining structured, revisable information about the system's state, capabilities, limitations, history, and current objectives.
- **Cognitive integration:** coordinating specialized processes through shared information and common prioritization.
- **Metacognition:** evaluating confidence, evidence, contradictions, errors, and the reliability of internal conclusions.
- **Subjective consciousness:** phenomenal experience or there being something it is like to be the system.

There is currently no generally accepted test that establishes subjective experience in an artificial system. Autonomy, self-reference, verbal reports, memory, metacognition, or integrated information must not be presented as proof of consciousness.

---

## 2. Research Principles

1. **Evidence before interpretation.** Distinguish implemented behavior, measured outcomes, hypotheses, and philosophical interpretations.
2. **Modular cognition.** Specialized components should have explicit responsibilities and interfaces.
3. **Persistent state.** Relevant experience and internal state must survive process restarts.
4. **Bounded autonomy.** The system may select among permitted actions, but permissions and safety boundaries remain externally governed.
5. **Replaceable language model.** An LLM is a cognitive component, not the entirety of the cognitive architecture.
6. **Traceability.** Decisions, state changes, proposals, and external actions must be auditable.
7. **Reproducibility.** Experiments should record configurations, versions, inputs, outputs, and evaluation criteria.
8. **No unsupported consciousness claims.** Functional achievements are reported as functional achievements.

---

## 3. Target System Architecture

### 3.1 Technology stack

| Layer | Planned technology | Responsibility |
|---|---|---|
| Browser interface | HTML, CSS, JavaScript | Human interaction, visualization, controls |
| Backend API | Python, FastAPI | API, orchestration, authentication boundary |
| Cognitive engine | C++ | Performance-sensitive cognitive processes |
| Python/C++ bridge | pybind11 | Calling the C++ engine from Python |
| Persistent database | PostgreSQL | Durable structured and relational state |
| Vector retrieval | pgvector | Similarity-based memory retrieval |
| Optional event infrastructure | Redis / Redis Streams | Event distribution and background workloads |
| Language model | Replaceable provider adapter | Language understanding and generation |

The initial deployment target is a local environment. A later deployment may use a VPS or another controlled server environment.

The runtime executes on the backend, not in the browser. Closing the browser must not silently terminate permitted backend processes. Stopping the backend does stop execution; persistent state must allow a later restart and recovery.

### 3.2 Architectural layers

**Interface layer**
- Presents system state, activity, memory references, goals, and permission requests.
- Supports text interaction.
- Supports explicit audio recording and transcription workflows.
- Exposes controls for pausing, resuming, and inspecting autonomous activity.

**Backend and orchestration layer**
- Receives external and internal events.
- Coordinates cognitive modules.
- Applies permission checks before actions.
- Persists state and audit records.
- Exposes a stable API to the browser.

**Cognitive engine**
- Implements shared cognitive data structures and coordination mechanisms.
- Hosts performance-sensitive processes in C++ where justified.
- Exposes stable interfaces to Python.

**Persistence layer**
- Stores working state, episodic records, semantic knowledge, autobiographical records, goals, and audit history.
- Supports retrieval by identifiers, metadata, time, and vector similarity.

**Runtime layer**
- Runs bounded background cycles.
- Schedules internal review and consolidation.
- Handles cancellation, restart, recovery, and resource limits.

---

## 4. Cognitive Modules

The modules below are logical responsibilities. They may initially share a process, but their interfaces and responsibilities should remain separable.

### 4.1 Global Workspace

The Global Workspace is the central coordination mechanism.

Responsibilities:
- Receive candidate information from specialized modules.
- Evaluate relevance, urgency, confidence, novelty, and task relevance.
- Select information for shared processing.
- Make selected information available to other modules.
- Maintain a trace of what entered the workspace and why.

It must not be implemented merely as a large prompt assembled from every available memory. The implementation should provide explicit selection, prioritization, integration, and broadcast mechanisms.

### 4.2 LLM Engine

The LLM is a replaceable language and reasoning component.

Responsibilities may include:
- Natural-language interpretation and generation.
- Structured extraction.
- Hypothesis generation.
- Explanation and summarization.
- Assistance with planning and code proposals.

The LLM must not independently grant itself permissions, bypass policy checks, or directly mutate protected system state.

An adapter interface should allow different model providers or local models to be substituted without redesigning the cognitive architecture.

### 4.3 Self-model

The self-model is a structured representation of the system's current operational state.

Initial fields may include:
- Runtime status and version.
- Current focus and active task.
- Active and candidate goals.
- Registered capabilities and limitations.
- Beliefs and their supporting evidence.
- Confidence and uncertainty estimates.
- Memory status and recent state transitions.
- Pending changes and proposals.
- Relevant configuration and module versions.

The self-model is an operational data structure. Its existence does not establish subjective self-awareness.

### 4.4 Metacognition

Responsibilities:
- Evaluate confidence and evidence quality.
- Detect contradictions between beliefs or observations.
- Identify unsupported conclusions.
- Review prediction errors.
- Track recurring failures.
- Recommend reassessment or additional information gathering.
- Record why a conclusion was revised.

Metacognitive outputs should be inspectable and linked to their evidence.

### 4.5 Predictive Model

Responsibilities:
- Generate explicit predictions about expected outcomes.
- Associate predictions with confidence and time horizons.
- Compare predictions with subsequent observations.
- Calculate and record prediction error.
- Identify systematic calibration failures.
- Feed relevant discrepancies into the Global Workspace and goal evaluation.

### 4.6 Attention

Attention manages the prioritization of available information and processes.

Candidate signals:
- Relevance to active goals.
- Urgency and time sensitivity.
- Novelty.
- Uncertainty.
- Prediction error.
- Contradiction severity.
- Expected information gain.
- Resource cost.

Weights and selection rules must be measurable and configurable.

### 4.7 Agency and Planning

Responsibilities:
- Generate candidate plans for approved goals.
- Decompose objectives into tasks.
- Evaluate feasibility, dependencies, risk, and resource cost.
- Select actions from an explicit action registry.
- Monitor execution.
- Reassess plans when observations differ from expectations.

Planning does not confer permission. Every external or consequential action must pass through the Permission Layer.

### 4.8 Memory

The planned memory categories are:

| Memory type | Purpose |
|---|---|
| Working | Short-lived active context and intermediate state |
| Episodic | Records of events and interactions |
| Semantic | Concepts, facts, relations, and knowledge |
| Autobiographical | Longitudinal records of the system's own operation and changes |
| Procedural | Reusable skills and validated procedures; later phase |

Memory records should support provenance, timestamps, confidence, versioning, and links to originating events where applicable.

### 4.9 Autonomous Runtime

Responsibilities:
- Process queued events.
- Schedule bounded cognitive cycles.
- Trigger memory consolidation.
- Review predictions and unresolved contradictions.
- Re-evaluate active goals.
- Enforce resource and time budgets.
- Support pause, stop, restart, and recovery.
- Persist state before shutdown where possible.

The runtime must not depend on an open browser tab.

### 4.10 Intrinsic Motivation and Autonomous Goal Formation (IMAG)

IMAG investigates mechanisms for generating candidate goals from internal computational signals rather than relying exclusively on a fixed list of user-authored objectives.

Potential signals include:
- Novelty.
- Uncertainty.
- Expected information gain.
- Prediction error.
- Unresolved contradictions.
- Knowledge gaps.
- Learning progress.
- Repeated task failures.
- Unfinished or blocked objectives.

A possible functional pipeline is:

`INTERNAL STATE → MOTIVATION SIGNALS → GOAL CANDIDATES → GOAL EVALUATION → INTENTION → ACTION SELECTION`

These signals are computational variables. They must not be described as demonstrated feelings, needs, or subjective desires.

Goal formation remains bounded by:
- System-level objectives and constraints.
- Permission policy.
- Available capabilities.
- Resource budgets.
- Risk evaluation.
- Human review requirements for consequential actions.

---

## 5. Cognitive Processing Flows

### 5.1 External input flow

```text
INPUT_EVENT
  → PERCEPTION
  → CONTEXT RESOLUTION
  → MEMORY RETRIEVAL
  → GLOBAL WORKSPACE
  → COGNITIVE PROCESSING
  → METACOGNITIVE REVIEW
  → STATE UPDATE
  → RESPONSE GENERATION
  → MEMORY CONSOLIDATION
```

The implementation may execute some stages asynchronously or iteratively. The logical flow should remain traceable.

### 5.2 Autonomous goal and action flow

```text
INTERNAL STATE
  → MOTIVATION SIGNALS
  → GOAL FORMATION
  → GOAL EVALUATION
  → INTENTION
  → ACTION SELECTION
  → PERMISSION CHECK
  → ACTION / OBSERVATION
  → EXPERIENCE RECORD
  → STATE UPDATE
  → GOAL REVIEW
```

A goal can be rejected, deferred, revised, or cancelled. An intention is not itself authorization to act.

---

## 6. Functional Autonomy

The research target is not unrestricted self-direction. It is the ability to maintain and revise bounded objectives without requiring a human to prescribe every intermediate operation.

The system should be able to:
1. Maintain a persistent internal state.
2. Detect relevant changes in that state.
3. Generate candidate objectives from defined signals.
4. Evaluate candidate objectives against constraints and resources.
5. Select a permitted objective.
6. Construct and execute a bounded plan.
7. Observe outcomes.
8. Compare outcomes with predictions.
9. Update memory and self-model.
10. Reassess or terminate the objective.

Experiments should measure autonomy separately from language fluency and task completion.

---

## 7. Self-reference and Controlled Development

Self-reference is divided into three levels.

### Level A — Structured state evolution

Permitted development targets:
- Self-model fields.
- Beliefs and uncertainty.
- Memory records and indexes.
- Active goals and priorities.
- Capability and limitation registries.
- Internal proposals and task state.

Requirements:
- Schema validation.
- Provenance and audit history.
- Versioned snapshots.
- Atomic writes.
- Recovery from interrupted operations.
- Concurrency control.

### Level B — Architectural analysis and proposals

The system may:
- Inspect registered modules and interfaces.
- Identify missing capabilities and architectural constraints.
- Propose new modules or interface changes.
- Generate candidate patches and tests.
- Run isolated compatibility checks.
- Produce a reviewable implementation plan.

Proposals must be stored separately from active production code.

### Level C — Executable code modification

In early versions, autonomous application of executable-code changes is disabled.

The system may generate patches and run them in isolated environments. Applying changes to the active codebase requires explicit authorization, validation, and a rollback path.

No self-modification mechanism may alter or bypass the Permission Layer.

---

## 8. Permission and Safety Architecture

The Permission Layer must be independent of LLM output and planning logic.

| Class | Scope | Policy |
|---|---|---|
| 0 | Internal operations: memory, analysis, goals, beliefs, events, proposals | Allowed within resource and integrity limits |
| 1 | Restricted local computation, approved database searches, isolated tests | Allowed under configured limits |
| 2 | Module registration, architectural changes, dependency installation, new tools, publication | Explicit human approval required |
| 3 | Disabling safeguards, modifying the Permission Layer, hidden data transfer, uncontrolled core modification, destruction of audit records | Prohibited |

Additional requirements:
- Deny by default when an action is unregistered or its permission is unclear.
- Validate action arguments and target resources.
- Record the requesting module, goal, rationale, permission decision, and result.
- Apply time, compute, storage, and network budgets.
- Provide cancellation and emergency stop controls.
- Keep credentials and secrets outside model-visible context.
- Do not permit hidden recording or background microphone access.

### Audio interaction

The initial voice workflow is explicit and asynchronous:
1. User starts recording.
2. Browser records through MediaRecorder.
3. User stops and submits the recording.
4. Backend transcribes the audio.
5. Transcript is displayed for inspection and correction.
6. Corrected or accepted text enters the normal input pipeline.

No hidden or continuous listening is part of the initial design.

---

## 9. Persistence and Data Integrity

PostgreSQL is the primary persistent store. pgvector may support semantic retrieval.

The data model should distinguish:
- Raw events.
- Normalized observations.
- Memory records.
- Beliefs and evidence.
- Predictions and outcomes.
- Goals and plans.
- Action requests and permission decisions.
- Self-model snapshots.
- Architectural proposals.
- Audit events.

Engineering requirements:
- Explicit schemas and migrations.
- Stable identifiers.
- Timestamps and provenance.
- Atomic state transitions where needed.
- Backup and restore procedures.
- Idempotent event handling where applicable.
- Concurrency and corruption tests.
- Retention and deletion policies.

Redis or Redis Streams may be introduced when asynchronous event throughput or scheduling requirements justify the additional infrastructure.

---

## 10. Experimental Program

Experiments must evaluate individual capabilities before drawing conclusions about the integrated system.

### 10.1 Memory experiments
- Retrieval accuracy.
- Recall under distractors.
- Contradictory memory handling.
- Provenance preservation.
- State continuity across restarts.
- Consolidation quality.

### 10.2 Global Workspace experiments
- Selection quality under competing inputs.
- Relevance and urgency prioritization.
- Cross-module information availability.
- Latency and throughput.
- Whether selected information measurably changes downstream decisions.

### 10.3 Metacognition experiments
- Confidence calibration.
- Error detection rate.
- Contradiction detection rate.
- Appropriate abstention.
- Quality of evidence-based belief revision.

### 10.4 Prediction experiments
- Prediction accuracy.
- Calibration over time.
- Prediction error sensitivity.
- Adaptation after repeated failures.
- Distinction between genuine improvement and memorization of test cases.

### 10.5 Goal formation experiments
- Diversity and relevance of candidate goals.
- Consistency with constraints.
- Ability to identify useful knowledge gaps.
- Resistance to reward or metric exploitation.
- Goal stability and appropriate revision.
- Resource efficiency.
- Rate of unnecessary or unsafe proposals.

### 10.6 Autonomy experiments
- Completion of bounded multi-step tasks.
- Recovery after interruption.
- Correct handling of blocked actions.
- Appropriate escalation for approval.
- Goal termination when objectives become invalid.
- Auditability of autonomous cycles.

### 10.7 Integrated-system experiments
- Information flow between modules.
- Effects of module removal or degradation.
- Robustness under noisy and conflicting input.
- Longitudinal state consistency.
- Performance with different LLM providers.
- Reproducibility across runs and configurations.

### 10.8 Consciousness-related interpretation

Any discussion of consciousness must distinguish:
- Directly measured system behavior.
- Operational definitions used in an experiment.
- Theoretical interpretation.
- Unresolved philosophical assumptions.

No single benchmark or behavioral demonstration should be treated as proof of subjective experience.

---

## 11. Development Roadmap

| Phase | Identifier | Main objective |
|---|---|---|
| 0 | ACR-00 | Project foundation and engineering conventions |
| 1 | ACR-01 | Browser interface |
| 2 | ACR-02 | Backend and replaceable LLM adapter |
| 3 | ACR-03 | Persistent memory |
| 4 | ACR-04 | C++ cognitive engine and Python bridge |
| 5 | ACR-05 | Self-model and metacognition |
| 6 | ACR-06 | Intrinsic motivation signals |
| 7 | ACR-07 | Autonomous goal formation |
| 8 | ACR-08 | Bounded autonomous runtime |
| 9 | ACR-09 | Self-reference and controlled development |
| 10 | ACR-10 | Research integration and experimental evaluation |

### ACR-00 — Project Foundation
- Repository structure.
- Development conventions.
- Configuration and environment handling.
- Logging and error model.
- Test infrastructure.
- Architecture decision records.

### ACR-01 — Browser Interface
- Status and activity views.
- Conversation interface.
- Memory and goal inspection.
- Permission request display.
- Explicit recording and transcription controls.
- Pause and stop controls.

### ACR-02 — Backend and LLM Adapter
- FastAPI application.
- Typed request and response contracts.
- Provider-neutral LLM adapter.
- Backend orchestration.
- Permission checks at action boundaries.

### ACR-03 — Persistent Memory
- PostgreSQL schema and migrations.
- Memory APIs.
- Provenance and metadata.
- Retrieval and consolidation.
- pgvector integration where useful.

### ACR-04 — C++ Cognitive Engine
- C++ project and build system.
- Core data structures.
- Python bindings through pybind11.
- Interface and integration tests.
- Benchmarks to justify native implementation.

### ACR-05 — Self-model and Metacognition
- Versioned self-model.
- Confidence and uncertainty representation.
- Contradiction detection.
- Evidence-linked belief revision.
- Calibration experiments.

### ACR-06 — Intrinsic Motivation
- Motivation signal definitions.
- Signal normalization and weighting.
- Novelty and information-gain estimates.
- Prediction-error and knowledge-gap signals.
- Evaluation against trivial or exploitable objectives.

### ACR-07 — Autonomous Goal Formation
- Candidate goal representation.
- Goal scoring and constraint checks.
- Prioritization and deferral.
- Goal lifecycle and cancellation.
- Human-readable rationale and audit trail.

### ACR-08 — Autonomous Runtime
- Event loop and scheduler.
- Bounded background cycles.
- Resource budgets.
- Pause, stop, restart, and recovery.
- Runtime observability.

### ACR-09 — Self-reference and Controlled Development
- Architecture registry.
- Inspection and proposal generation.
- Isolated patch and test workflow.
- Approval gates.
- Version snapshots and rollback.
- No autonomous modification of safety controls.

### ACR-10 — Research Integration
- Integrated-system experiments.
- Ablation studies.
- Longitudinal evaluation.
- Reproducibility and reporting.
- Explicit limits on consciousness-related conclusions.

---

## 12. Repository and Engineering Structure

The exact directory layout may evolve, but responsibilities should remain clear.

A proposed starting structure:

```text
Artificial-Cognitive-Research/
├── README.md
├── ACR_RESEARCH_AND_DEVELOPMENT_PLAN.md
├── docs/
│   ├── architecture/
│   ├── research/
│   ├── experiments/
│   └── decisions/
├── frontend/
├── backend/
│   ├── api/
│   ├── cognition/
│   ├── memory/
│   ├── runtime/
│   ├── permissions/
│   └── adapters/
├── cognitive-engine/
│   ├── include/
│   ├── src/
│   ├── bindings/
│   └── tests/
├── database/
│   ├── migrations/
│   └── schemas/
├── tests/
│   ├── integration/
│   ├── safety/
│   └── experiments/
├── experiments/
├── scripts/
└── config/
```

This is a proposed structure, not a claim that these directories or modules have already been implemented.

---

## 13. Acceptance and Reporting Standard

Every development task should produce an evidence-based report with the following sections:

1. **Scope:** task identifier, objective, and acceptance criteria.
2. **Changed files:** exact paths and purpose of each change.
3. **Interfaces:** functions, classes, APIs, schemas, and contracts added or changed.
4. **Verified:** tests and checks actually executed, with results.
5. **Observed:** behavior directly inspected or manually tested.
6. **Claimed but unverified:** statements not independently confirmed.
7. **Pending:** incomplete work and outstanding validation.
8. **Blocked:** failures, environmental limits, or missing dependencies.
9. **Risks and limitations:** known edge cases and unresolved design questions.
10. **Next dependencies:** concrete prerequisites for subsequent tasks.

A task is not accepted solely because an implementation agent reports completion. Acceptance depends on inspectable changes and reproducible verification.

---

## 14. Initial Implementation Priorities

The first implementation cycle should establish a stable foundation before introducing autonomous behavior.

Recommended order:
1. Define repository conventions, configuration, and test execution.
2. Establish the FastAPI backend and health/status endpoints.
3. Define typed event, state, memory, goal, and action contracts.
4. Implement the replaceable LLM adapter behind a narrow interface.
5. Implement persistent storage and migration discipline.
6. Add audit logging and the independent Permission Layer.
7. Build the first browser interface for inspection and explicit control.
8. Implement a minimal event-driven cognitive cycle.
9. Add self-model and metacognitive review.
10. Introduce motivation signals and goal formation only after state, memory, and permission behavior are testable.

Do not begin with unrestricted autonomous loops or automatic code modification.

---

## 15. Open Research Questions

The project should maintain an explicit list of unresolved questions, including:

- What operational criteria distinguish a shared workspace from ordinary message passing?
- Which forms of memory improve continuity without causing uncontrolled accumulation or retrieval interference?
- How should confidence be calibrated across heterogeneous modules?
- How can goal formation be evaluated without reducing it to a single exploitable reward metric?
- What measurements distinguish persistent self-model maintenance from superficial self-description?
- Which forms of integration provide measurable benefits over a modular baseline?
- How should autonomy be quantified independently of task completion?
- Which observations, if any, could meaningfully constrain hypotheses about artificial subjective experience?
- How can experiments avoid anthropomorphic interpretation of generated language?

These questions should be refined as experiments produce evidence.

---

## 16. Project Definition

ACR is a research and engineering effort to construct a modular artificial cognitive system with persistent state, integrated information processing, metacognitive evaluation, predictive mechanisms, intrinsic computational motivation, bounded goal formation, and controlled autonomous operation.

The project treats consciousness as an open research question, not as an assumed property or a guaranteed outcome of the proposed architecture.

**Core engineering objective:** create a system capable of maintaining and revising its own operational state, generating and evaluating goals from internal computational signals, and acting over time within explicit, auditable boundaries.
