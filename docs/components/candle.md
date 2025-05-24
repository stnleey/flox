# Candle

The `Candle` struct represents a time-based OHLCV (Open, High, Low, Close, Volume) candlestick.  
It is used to summarize trade data over a specific time interval.

## Purpose

To provide a compact, aggregated view of price and volume action within a time window.

## Struct Definition

```cpp
struct Candle {
  double open = 0.0;
  double high = 0.0;
  double low = 0.0;
  double close = 0.0;
  double volume = 0.0;
  std::chrono::system_clock::time_point startTime;
  std::chrono::system_clock::time_point endTime;
};
```

## Fields

- `open`: first trade price in the interval
- `high`: highest trade price
- `low`: lowest trade price
- `close`: last trade price
- `volume`: total traded volume
- `startTime`: beginning of the time interval
- `endTime`: end of the time interval

## Use Cases

- Strategy input (e.g. momentum or mean-reversion logic)
- Visualization and charting
- Statistical modeling

## Notes

- Time is tracked using `std::chrono::system_clock`
- Typically constructed and emitted by the `CandleAggregator`