# Writing Strategies

Strategies in Flox are implemented by subclassing `IStrategy`, which provides a unified interface for receiving market data and submitting orders.

## Strategy Interface

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
};
```

### Core Methods

- `onStart() / onStop()` — lifecycle hooks when engine starts or stops.
- `onCandle(...)` — receives completed candles.
- `onTrade(...)` — receives trade data.
- `onBookUpdate(...)` — receives order book updates.

These callbacks are called by the engine for each symbol the strategy is subscribed to.

## Optional Components

The strategy can interact with other engine components using these setters:

- `setOrderExecutor(...)` — required to place orders.
- `setRiskManager(...)` — optional; if used, `allow(order)` will be checked before submission.
- `setPositionManager(...)` — optional; provides position tracking.
- `setOrderValidator(...)` — optional; if set, `validate(order, reason)` is called before submission.

## Submitting Orders

If a strategy wants to submit an order:

```cpp
if (_riskManager->allow(order) && _validator->validate(order, reason)) {
    _executor->submitOrder(order);
}
```

This ensures only valid and approved orders are sent to the market.

## Best Practices

- Avoid heavy computations in market data callbacks.
- Keep internal state clean and minimal.
- Use position manager for stateful strategies only if needed.

## Example

```cpp
class MyStrategy : public IStrategy {
public:
  void onBookUpdate(const BookUpdate &update) override {
    // Trading logic
    Order o = {...};
    if (GetOrderValidator() && !GetOrderValidator()->validate(o, _reason)) return;
    if (GetRiskManager() && !GetRiskManager()->allow(o)) return;
    GetOrderExecutor()->submitOrder(o);
  }
};
```