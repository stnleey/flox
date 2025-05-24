# WindowedOrderBook

`WindowedOrderBook` is a high-performance implementation of `IOrderBook` that maintains a centered price window of limited depth.  
It is optimized for speed, memory efficiency, and cache locality in latency-sensitive environments.

## Purpose

To track a limited range of price levels around the center price, making it ideal for real-time trading systems.

## Class Definition

```cpp
class WindowedOrderBook : public IOrderBook {
public:
  WindowedOrderBook(double tickSize, double expectedDeviation,
                    std::pmr::memory_resource *mem);

  void applyBookUpdate(const BookUpdate &update) override;

  size_t priceToIndex(double price) const;
  double indexToPrice(size_t index) const;
  bool isPriceInWindow(double price) const;

  double bidAtPrice(double price) const override;
  double askAtPrice(double price) const override;

  std::optional<double> bestBid() const override;
  std::optional<double> bestAsk() const override;

  double getBidQuantity(double price) const;
  double getAskQuantity(double price) const;

  void printBook(std::size_t depth = 10) const;
  double centerPrice() const;
};
```

## Key Concepts

- Maintains a fixed-size ring buffer of price levels centered around `_centerPrice`
- Bid and ask sides are implemented using `BookSide`
- `tickSize` and `expectedDeviation` define price granularity and window size

## Performance Considerations

- Fast lookup and updates via index mapping (`priceToIndex`)
- Memory-local storage for high CPU cache efficiency
- Thread-safe via internal `std::mutex`

## Use Cases

- Used in live strategies where full book depth is unnecessary
- Engine-level real-time book tracking and signal generation
- Suitable for DLOB-style matching and top-of-book decision logic

## Notes

- Incoming book updates outside the window trigger `shiftWindow(...)`
- Prices must be aligned to tick size for correctness