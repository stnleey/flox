# Strategy Interface

The `IStrategy` interface defines the base structure for implementing trading strategies in Flox.  
It provides lifecycle hooks and market event handlers, and grants access to execution, risk, and position services.

## Purpose

To create modular, pluggable strategies that react to market data and can submit orders through controlled interfaces.

## Interface Definition

```cpp
class IStrategy {
public:
  virtual ~IStrategy() = default;

  virtual void onStart();
  virtual void onStop();

  virtual void onCandle(SymbolId symbol, const Candle &candle);
  virtual void onTrade(const Trade &trade);
  virtual void onBookUpdate(const BookUpdate &bookUpdate);

  void setPositionManager(IPositionManager *manager);
  void setRiskManager(IRiskManager *manager);
  void setOrderExecutor(IOrderExecutor *executor);
  void setOrderValidator(IOrderValidator *validator);

protected:
  IRiskManager *GetRiskManager();
  IPositionManager *GetPositionManager();
  IOrderExecutor *GetOrderExecutor();
  IOrderValidator *GetOrderValidator();
};
```

## Responsibilities

- Reacts to market data via `onCandle`, `onTrade`, and `onBookUpdate`
- Optionally implements `onStart` / `onStop` for initialization/teardown
- May use `IOrderExecutor` to place orders and `IRiskManager`/`IOrderValidator` to enforce logic
- Has access to current position via `IPositionManager`

## Notes

- All dependencies are injected via `setX()` methods
- Strategies are typically managed by `StrategyManager`
- Designed for subclassing: each concrete strategy inherits from `IStrategy`