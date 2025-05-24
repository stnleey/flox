# Kill Switch

The `IKillSwitch` interface provides a mechanism for detecting critical execution conditions and halting trading activity.

## Purpose

A kill switch is used to protect the system from cascading failures, excessive losses, or abnormal behavior by forcibly stopping order flow.

## Interface Definition

```cpp
class IKillSwitch {
public:
  virtual ~IKillSwitch() = default;

  virtual void check(const Order &order) = 0;
  virtual void trigger(const std::string &reason) = 0;
  virtual bool isTriggered() const = 0;
  virtual std::string reason() const = 0;
};
```

## Responsibilities

- `check(...)` allows pre-trade validation for dangerous or outlier orders
- `trigger(...)` forcibly activates the kill switch with a descriptive reason
- `isTriggered()` returns current status
- `reason()` provides context for the shutdown

## Use Cases

- Manual or automatic shutdowns under emergency
- Circuit breakers for latency spikes, volume anomalies, or rejected order bursts
- Integration with `IOrderExecutor` or `IRiskManager`

## Notes

- The kill switch is a critical risk control tool in production systems
- It is expected to be queried before submitting any order