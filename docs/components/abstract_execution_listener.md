# Order Execution Listener

The `IOrderExecutionListener` interface allows components to receive notifications when an order is filled or rejected.

## Purpose

This interface is used to propagate execution events to listeners, typically for updating internal state or triggering follow-up logic.

## Interface Definition

```cpp
class IOrderExecutionListener {
public:
  virtual ~IOrderExecutionListener() = default;
  virtual void onOrderFilled(const Order &order) = 0;
  virtual void onOrderRejected(const Order &order, const std::string &reason) = 0;
};
```

## Typical Implementations

- `PositionManager`: updates symbol positions upon order fills
- Custom loggers or analytics modules: record order results

## Notes

- Can be used with `MultiExecutionListener` to dispatch to multiple consumers
- Order rejections can include additional metadata via the `reason` string