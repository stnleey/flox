# Order Executor

The `IOrderExecutor` is the central interface for submitting and managing orders in the Flox framework.  
It acts as the bridge between the strategy layer and the actual execution venue or simulator.

## Responsibilities

- Accepts and submits orders
- Notifies listeners when orders are filled or rejected
- Optionally tracks execution metrics via a tracker
- Integrates into the system lifecycle via `ISubsystem`

## Class Definition

```cpp
class IOrderExecutor : public ISubsystem {
public:
  virtual ~IOrderExecutor() = default;

  virtual void submitOrder(const Order &order) = 0;

  void setExecutionTracker(IExecutionTracker *tracker);
  void setListener(IOrderExecutionListener *listener);

protected:
  IExecutionTracker *getExecutionTracker() const;
  IOrderExecutionListener *getListener() const;
};
```

## Usage

- Strategies call `submitOrder(...)` to execute trades.
- `setListener(...)` must be used to register a consumer for order events.
- Optionally, `setExecutionTracker(...)` can be used to collect latency metrics.

## Notes

- Execution logic is implemented in derived classes (e.g., mock or real-time executors).
- Listener and tracker pointers are nullable; behavior should be guarded accordingly.