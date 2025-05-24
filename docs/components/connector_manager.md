# ConnectorManager

The `ConnectorManager` is a utility class responsible for managing and orchestrating multiple `ExchangeConnector` instances.  
It handles startup, callback wiring, and centralized control of market data ingestion.

## Purpose

To centralize exchange connector lifecycle management and routing of book and trade data.

## Class Definition

```cpp
class ConnectorManager {
public:
  void registerConnector(std::shared_ptr<ExchangeConnector> connector);

  void startAll(std::function<void(const BookUpdate &)> onBookUpdate,
                std::function<void(const Trade &)> onTrade);
};
```

## Responsibilities

- Registers connectors by `exchangeId`
- Wires unified `BookUpdate` and `Trade` callbacks to each connector
- Starts all registered connectors

## Use Cases

- Bootstrapping all active exchange streams at engine startup
- Multiplexing book/trade events into a single processing pipeline
- Test harnesses and connector orchestration

## Notes

- Internally stores connectors in a `std::map<std::string, std::shared_ptr<...>>`
- Logs connector starts to stdout
- Expects connectors to support `setCallbacks(...)` and `start()`