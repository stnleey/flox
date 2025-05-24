# ISubsystem and Subsystem<T>

The `ISubsystem` interface represents a component with a well-defined start/stop lifecycle.  
`Subsystem<T>` is a generic wrapper that adapts any compatible component to conform to `ISubsystem`.

## Purpose

To enable modular lifecycle management of engine components.

## Interface Definition

```cpp
class ISubsystem {
public:
  virtual ~ISubsystem() = default;
  virtual void start() = 0;
  virtual void stop() = 0;
};
```

## Template Wrapper

```cpp
template <typename T>
class Subsystem : public ISubsystem {
public:
  explicit Subsystem(std::unique_ptr<T> impl);

  void start() override;
  void stop() override;

  T *get() const;
};
```

## Responsibilities

### `ISubsystem`

- Defines the basic start/stop interface for engine-managed components
- Ensures consistent lifecycle control for execution, routing, logging, etc.

### `Subsystem<T>`

- Adapts any class with `start()` and `stop()` methods to conform to `ISubsystem`
- Uses compile-time check (`if constexpr`) to guard optional method presence
- Owns the underlying component via `std::unique_ptr`

## Use Cases

- Strategy managers, risk modules, sinks, routers — all can implement or be wrapped as subsystems
- Managed startup/shutdown via engine orchestration

## Notes

- `Subsystem<T>` allows non-virtual classes to be managed uniformly
- Wrappers ensure loosely coupled lifecycle coordination