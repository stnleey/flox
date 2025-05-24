# ExchangeConnector

The `ExchangeConnector` class defines an abstract interface for connecting to and receiving data from external trading venues.  
It serves as the foundation for real or simulated exchange integrations.

## Purpose

To provide a standard API for:
- Subscribing to order book and trade data
- Starting/stopping market data streams
- Identifying the source of data

## Class Definition

```cpp
class ExchangeConnector {
public:
  virtual ~ExchangeConnector() = default;

  using BookUpdateCallback = std::function<void(const BookUpdate &)>;
  using TradeCallback = std::function<void(const Trade &)>;

  virtual void start() = 0;
  virtual void stop() = 0;

  virtual std::string exchangeId() const = 0;

  virtual void setCallbacks(BookUpdateCallback onBookUpdate,
                            TradeCallback onTrade);
};
```

## Responsibilities

- Implements `start()` and `stop()` lifecycle for data streaming
- Delivers `BookUpdate` and `Trade` data via registered callbacks
- Identifies itself via `exchangeId()`

## Usage

- Subclasses (e.g. `BybitConnector`, `MockConnector`) override the abstract methods
- `setCallbacks()` connects internal emitters to external handlers
- Use `emitBookUpdate(...)` and `emitTrade(...)` to dispatch data

## Notes

- Callbacks are optional but should be set before `start()`
- Thread safety and reconnect logic are the responsibility of the derived class