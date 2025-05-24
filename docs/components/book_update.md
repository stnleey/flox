# BookUpdate

`BookUpdate` is a data structure used to represent changes in the order book.  
It supports both full snapshots and incremental delta updates with symbol-specific and timestamped context.

## Purpose

To transport changes in order book state (either full or partial) between components in a standardized, memory-efficient format.

## Type Definition

```cpp
enum class BookUpdateType { SNAPSHOT, DELTA };

struct BookLevel {
  double price;
  double quantity;

  BookLevel() = default;
  BookLevel(double p, double q);
};

struct BookUpdate {
  SymbolId symbol;
  BookUpdateType type;
  std::pmr::vector<BookLevel> bids;
  std::pmr::vector<BookLevel> asks;
  std::chrono::system_clock::time_point timestamp;

  BookUpdate(std::pmr::memory_resource *res);
};
```

## Responsibilities

- Encapsulates book data per symbol
- Distinguishes between full state (`SNAPSHOT`) and incremental changes (`DELTA`)
- Separates bid and ask levels
- Allocates via `std::pmr::memory_resource` for deterministic memory control

## Use Cases

- Passed into `IOrderBook::applyBookUpdate(...)`
- Emitted from exchange connectors
- Logged or recorded for replay/backtesting

## Notes

- `bids` and `asks` are sorted externally (not guaranteed by structure)
- Timestamp is generated externally (e.g., at data ingest time)