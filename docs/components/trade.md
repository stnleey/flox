# Trade

The `Trade` struct represents a single executed transaction in the market.  
It captures the essential information needed for trade-based analytics, candle aggregation, and strategy input.

## Purpose

To model the execution of a buy or sell order on a given symbol.

## Struct Definition

```cpp
struct Trade {
  SymbolId symbol;
  double price;
  double quantity;
  bool isBuy;
  std::chrono::system_clock::time_point timestamp;
};
```

## Fields

- `symbol`: ID of the traded symbol
- `price`: Executed price of the trade
- `quantity`: Quantity traded
- `isBuy`: Direction of the trade (`true` for buy, `false` for sell)
- `timestamp`: Time at which the trade occurred

## Use Cases

- Input for `CandleAggregator`
- Strategy triggers based on recent market trades
- Backtesting and performance visualization

## Notes

- `SymbolId` is resolved via `SymbolRegistry`
- Timestamp is system clock based; ensure synchronization if comparing across sources