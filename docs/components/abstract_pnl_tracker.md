# PnL Tracker

The `IPnLTracker` interface tracks realized profit and loss based on executed orders.  
It is designed to be notified whenever an order is filled, allowing cumulative performance accounting.

## Purpose

To measure realized profitability in trading strategies or the entire system.

## Interface Definition

```cpp
class IPnLTracker {
public:
  virtual ~IPnLTracker() = default;
  virtual void onOrderFilled(const Order &order) = 0;
};
```

## Responsibilities

- Receives every filled order via `onOrderFilled(...)`
- Computes net PnL based on position changes, prices, and fees (if applicable)
- May support symbol-wise aggregation or global totals

## Use Cases

- Strategy-level performance reporting
- Centralized analytics dashboards
- Integration with monitoring or alerting systems

## Notes

- This interface is intentionally minimal
- Internal state (e.g., positions or entry prices) is managed by the implementation