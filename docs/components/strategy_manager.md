# Strategy Manager

The `StrategyManager` is a core subsystem in the Flox framework responsible for managing and routing events to trading strategies.

## Responsibilities

- Holds and manages multiple trading strategies
- Forwards market data (candles, trades, order book updates) to each strategy
- Links strategies to the position manager
- Provides lifecycle control (start/stop)

## Class Definition

```cpp
class StrategyManager : public ISubsystem {
public:
  void start() override;
  void stop() override;

  void setPositionManager(IPositionManager *pm);
  void addStrategy(const std::shared_ptr<IStrategy> &strategy);

  void onCandle(SymbolId symbol, const Candle &candle);
  void onTrade(const Trade &trade);
  void onBookUpdate(const BookUpdate &bookUpdate);
};
```

## Usage

- Use `addStrategy(...)` to register strategy instances.
- Market data should be routed via `onCandle`, `onTrade`, and `onBookUpdate`.

## Notes

- Strategies are expected to implement the `IStrategy` interface.
- `StrategyManager` does not take ownership of the `IPositionManager`.