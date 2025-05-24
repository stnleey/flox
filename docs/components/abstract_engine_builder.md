# IEngineBuilder

`IEngineBuilder` is the interface for building the Flox trading engine.  
It provides a flexible and modular way to configure and assemble the engine's core components.

## Purpose

To decouple engine construction logic from the `Engine` class itself, enabling different configurations (e.g., simulation, production, testing).

## Interface

```cpp
class IEngineBuilder {
public:
  virtual ~IEngineBuilder() = default;
  virtual std::unique_ptr<Engine> build() = 0;
};
```

## Responsibilities

- Construct and return a fully initialized instance of `Engine`
- Encapsulate the setup of subsystems, strategies, and connectors

## Example Usage

```cpp
DemoEngineBuilder builder(config);
auto engine = builder.build();
engine->start();
```

## Notes

Implementations are expected to handle:
- Registering symbols
- Creating and wiring up subsystems
- Loading strategy configurations
- Attaching connectors
