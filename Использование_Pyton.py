from cognitive_engine import CognitiveEngine, SignalType, MotivationSignal

engine = CognitiveEngine()
engine.initialize("0.1.0")

# Submit candidate with motivation signals
signals = [
    MotivationSignal(type=SignalType.PREDICTION_ERROR, value=0.8, weight=0.3, source_module="predictor"),
    MotivationSignal(type=SignalType.NOVELTY, value=0.6, weight=0.2, source_module="perception"),
]
engine.submit_to_workspace(id=1, content="Unexpected sensor reading", source="sensor_fusion", signals=signals)

# Process highest-priority item
item = engine.process_next()
if item:
    print(f"Selected: {item.content} (priority={item.priority_score:.3f})")

# Inspect self-model
state = engine.get_self_model()
print(f"Status: {state.runtime_status}, Version: {state.version}")

# Audit trail
for record in engine.get_audit_trail(5):
    print(f"[{record.module}] {record.action}: {record.details}")
