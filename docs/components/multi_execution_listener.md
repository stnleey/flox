# MultiExecutionListener

The `MultiExecutionListener` is a fan-out dispatcher for execution events.  
It enables multiple `IOrderExecutionListener` consumers to receive notifications for the same order events.

## Purpose

To allow multiple subsystems (e.g., position tracking, analytics, risk) to simultaneously observe order fills and rejections.

## Class Definition

```cpp
class MultiExecutionListener : public IOrderExecutionListener {
public:
  void addListener(IOrderExecutionListener *listener);

  void onOrderFilled(const Order &order) override;
  void onOrderRejected(const Order &order, const std::string &reason) override;
};
```

## Responsibilities

- Aggregates multiple execution listeners
- Broadcasts `onOrderFilled` and `onOrderRejected` events to all registered listeners
- Avoids duplicate registration

## Use Cases

- Forward execution events to both:
  - `PositionManager` (for state updates)
  - `ExecutionTracker` (for latency tracking)
- Composite behavior in modular systems
- Strategy instrumentation for backtesting or debugging

## Notes

- Listeners are stored as raw pointers (ownership not assumed)
- Safe from duplicate registration via internal check