# Risk Manager

The `IRiskManager` interface defines the logic responsible for enforcing trading risk limits before order submission.

## Purpose

To act as a gatekeeper for outbound orders, ensuring compliance with capital, exposure, and custom risk rules.

## Interface Definition

```cpp
class IRiskManager : public ISubsystem {
public:
  virtual ~IRiskManager() = default;
  virtual bool allow(const Order &order) const = 0;
};
```

## Responsibilities

- `allow(...)`: evaluates the order and returns `true` if it's safe to proceed
- Integrated into the system lifecycle via `ISubsystem`

## Use Cases

- Capital-based order blocking
- Exposure control per symbol or globally
- Custom constraints (e.g., time-of-day, user-defined logic)

## Notes

- Typically used in conjunction with `IOrderExecutor`
- Stateless or stateful implementations depending on enforcement logic