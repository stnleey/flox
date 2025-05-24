# Architecture

Flox is a modular framework for building low-latency execution systems. Its design emphasizes **separation of concerns**, **predictable performance**, and **composability**.

The architecture is split into two conceptual layers:

---

## 1. Abstract Layer

This layer defines **pure interfaces** (no logic, no state). It captures the essential contracts of the system:

- `IStrategy`: strategy logic
- `IOrderExecutor`: order submission
- `IOrderExecutionListener`: receives fills
- `IRiskManager`, `IOrderValidator`, `IPositionManager`
- `IOrderBook`, `ExchangeConnector`
- `ISubsystem`: unified lifecycle interface

### Why this matters

- Enables mocking, backtesting, and simulation
- Decouples business logic from transport and persistence
- Makes performance and implementation decisions orthogonal to correctness

---

## 2. Implementation Layer

This layer contains **stateful, optimized implementations** of the abstract interfaces. These are designed for:

- **Speed** (SIMD, cache locality, ring buffers)
- **Control** (manual memory management, preallocated arenas)
- **Isolation** (components don't share global state)

### Notable modules

- `WindowedOrderBook` — fast, centered price-level view
- `MarketDataBus` — efficient symbol-specific fan-out
- `CandleAggregator` — low-overhead OHLC aggregation
- `SymbolIdOrderRouter` — routing updates to N orderbooks
- `MultiExecutionListener` — multicast execution notification
- `EngineConfig` — static system configuration

---

## Lifecycle Design

Every runtime component inherits or is wrapped in `ISubsystem`, exposing `start()` and `stop()`.

This enables:

- Safe startup/shutdown sequences
- Controlled test environments
- Runtime orchestration (e.g., benchmarking, simulation, replay)

---

## Design Philosophy

### Minimal assumptions

Flox does not force any specific strategy architecture, market model, or execution venue. It gives you tools to build your own.

- You decide how orders are placed (through `IOrderExecutor`)
- You define what a “strategy” means (extend `IStrategy`)
- You handle execution routing and risk management

### Memory control

Where performance is key, Flox gives full control over memory allocation using `std::pmr`. Examples:

- `BookSide` uses ring buffers in arena-allocated memory
- `BookUpdate`/`Trade` objects avoid heap allocations
- `ArenaResource` supports preallocated bounded pools

### Symbol-centric architecture

All real-time operations are indexed by `SymbolId` — a compact, uint32_t identifier mapped from exchange:symbol strings.

This allows:

- High-speed routing and lookup
- Minimal memory usage
- Avoidance of string comparisons or allocations in hot paths

---

## Intended Use

Flox is not a turnkey system — it’s an **infrastructure layer**. You use it to build:

- Real-time execution engines
- Live market data processors
- Strategy sandboxes
- Replay/backtest frameworks
- Low-latency signal routers

It is ideal for C++ teams who need:
- Deterministic performance
- Modular structure
- Precise lifecycle management
- Full control over execution and memory

---

## Integration Example

```cpp
auto bus = std::make_shared<MarketDataBus>();
auto book = std::make_unique<WindowedOrderBook>(0.01, 1.0, arena);
auto router = std::make_shared<SymbolIdOrderRouter>(registry, bookFactory);
auto strategy = std::make_shared<MyStrategy>();

strategy->setOrderExecutor(executor);
strategy->setPositionManager(positionManager);

bus->subscribeToCandles(symbolId, [&](SymbolId s, const Candle &c) {
    strategy->onCandle(s, c);
});

router->route(update); // updates books
```

---

## Summary

Flox is modular by design. Its architecture enables:

- Safe extension and testing
- Deterministic low-latency behavior
- Fine-grained system composition
- Clear boundary between abstract logic and optimized runtime

Use it as a foundation — not a framework. You own the rules, Flox handles the wires.