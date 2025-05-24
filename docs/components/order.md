# Order

The `Order` struct represents a trade order in the Flox framework.  
It encapsulates direction, size, price, type, and timestamp for execution tracking and matching.

## Purpose

To serve as the fundamental unit of interaction between strategies, executors, and tracking components.

## Struct Definition

```cpp
struct Order {
  uint64_t id;
  Side side;
  double price;
  double quantity;
  OrderType type;
  SymbolId symbol;
  std::chrono::system_clock::time_point timestamp;
};
```

## Fields

- `id`: Unique identifier for the order
- `side`: Direction of the order (`BUY` or `SELL`)
- `price`: Price at which to buy/sell (ignored for market orders)
- `quantity`: Quantity to be traded
- `type`: Order type (`LIMIT` or `MARKET`)
- `symbol`: Symbol associated with the order
- `timestamp`: Time of order creation or submission

## Use Cases

- Passed from strategy to order executor
- Tracked by position and latency components
- Logged and serialized for auditing or backtesting

## Notes

- `id` must be generated externally and be unique
- For market orders, price can be set to `0.0` or ignored depending on executor logic