# FullOrderBook

The `FullOrderBook` is a concrete implementation of `IOrderBook` that maintains the complete bid and ask depth using sorted maps.  
It is designed for accuracy and completeness over performance.

## Purpose

To track the full state of an order book for a given symbol, including all price levels.

## Class Definition

```cpp
class FullOrderBook : public IOrderBook {
public:
  void applyBookUpdate(const BookUpdate &update) override;
  std::optional<double> bestBid() const override;
  std::optional<double> bestAsk() const override;

  double bidAtPrice(double price) const override;
  double askAtPrice(double price) const override;
};
```

## Responsibilities

- Applies full snapshots and delta updates to the book
- Stores bid levels in descending order using `std::map<double, double, std::greater<>>`
- Stores ask levels in ascending order using `std::map<double, double>`
- Provides best bid/ask prices and quantity at any given level

## Thread Safety

- All access and mutation is guarded by a `std::mutex`
- Safe for multi-threaded reading/writing

## Use Cases

- Historical replay or data capture
- Backtesting with full book state
- Accuracy-sensitive trading logic or visualization

## Notes

- Slower than windowed or top-of-book variants
- Designed for fidelity and simplicity, not speed