# StorageSink

The `StorageSink` interface defines a generic sink for storing executed orders.  
It is part of the Flox engine’s persistence and logging infrastructure.

## Purpose

To persist orders for later analysis, backtesting, compliance, or monitoring.

## Class Definition

```cpp
class StorageSink : public ISubsystem {
public:
  virtual ~StorageSink() = default;
  virtual void store(const Order &order) = 0;
};
```

## Responsibilities

- Accepts `Order` objects via `store(...)`
- Implements storage logic (e.g. MongoDB, file-based, in-memory)
- Acts as a pluggable subsystem

## Use Cases

- Write trades to a database for historical analysis
- Stream orders to a file or external telemetry system
- Capture order flow for backtesting or compliance

## Notes

- Must be connected to execution callbacks (e.g., via `IOrderExecutionListener`)
- Storage behavior is defined by derived implementations