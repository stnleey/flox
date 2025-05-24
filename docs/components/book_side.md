# BookSide

The `BookSide` class represents one side (bid or ask) of an order book.  
It is designed as a fixed-size, ring-buffer-style structure optimized for low-latency price level access and updates.

## Purpose

To efficiently maintain and manipulate one side of the limit order book, supporting rapid updates and best-level queries.

## Class Definition

```cpp
class BookSide {
public:
  enum class Side { Bid, Ask };

  BookSide(std::size_t windowSize, Side side, std::pmr::memory_resource *mem);

  void setLevel(std::size_t index, double qty);
  double getLevel(std::size_t index) const;

  void shift(int levels);
  void clear();

  std::optional<std::size_t> findBest() const;

  std::pmr::memory_resource *allocator() const;
};
```

## Responsibilities

- `setLevel(index, qty)`: sets quantity at a specific price level index
- `getLevel(index)`: retrieves quantity at index
- `shift(levels)`: shifts the book window by a relative offset
- `clear()`: resets all levels
- `findBest()`: finds the best price level (highest bid or lowest ask)

## Internal Design

- Ring buffer backed by `std::pmr::vector<double>`
- Offset-based indexing via `ring(...)`
- Tracks best index to avoid scanning entire window

## Use Cases

- Used internally in `WindowedOrderBook` and related components
- Performance-sensitive scenarios requiring rapid top-of-book updates

## Notes

- Memory resource passed via constructor enables allocator injection