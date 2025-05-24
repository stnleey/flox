# MarketDataBus

The `MarketDataBus` is a publish-subscribe router for market data in the Flox engine.  
It allows multiple components to independently subscribe to updates for candles, trades, or order books.

## Purpose

To decouple market data producers (e.g., connectors) from consumers (e.g., strategies, aggregators), while supporting symbol-specific subscriptions.

## Class Definition

```cpp
class MarketDataBus : public ISubsystem {
public:
  using CandleCallback = std::function<void(SymbolId, const Candle &)>;
  using TradeCallback = std::function<void(const Trade &)>;
  using BookUpdateCallback = std::function<void(const BookUpdate &)>;

  enum class SubscriptionType { Candle, Trade, BookUpdate };

  struct SubscriptionHandle {
    SymbolId symbol;
    SubscriptionType type;
    size_t index;
    bool operator==(const SubscriptionHandle &other) const;
  };

  SubscriptionHandle subscribeToCandles(SymbolId symbol, CandleCallback cb);
  SubscriptionHandle subscribeToTrades(SymbolId symbol, TradeCallback cb);
  SubscriptionHandle subscribeToBookUpdates(SymbolId symbol, BookUpdateCallback cb);

  void unsubscribe(const SubscriptionHandle &handle);

  void onCandle(SymbolId symbol, const Candle &candle);
  void onTrade(const Trade &trade);
  void onBookUpdate(const BookUpdate &update);

  void clear();
  void start() override;
  void stop() override;
};
```

## Responsibilities

- Manages multiple subscribers per symbol and event type
- Emits incoming data to appropriate callbacks
- Supports unsubscribe via `SubscriptionHandle`
- Thread-safe via internal mutex

## Internal Design

- Uses a `Router` struct to hold subscriber lists per symbol
- Callbacks are stored in `std::vector<std::optional<...>>` for efficient access and removal
- Synchronization is handled with `std::mutex`

## Use Cases

- Strategies subscribing to specific symbols
- Centralized data distribution for logging, analytics, or aggregation
- Decoupling modules with dynamic subscription capabilities

## Notes

- Subscriptions are retained until explicitly unsubscribed or `clear()` is called
- Subscribers must avoid holding long locks or throwing exceptions inside callbacks