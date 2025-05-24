# OrderRouter & SymbolIdOrderRouter

The `IOrderRouter` interface defines how order book updates are dispatched to symbol-specific order books.  
`SymbolIdOrderRouter` is a concrete implementation that uses a `SymbolId` map and a shared `IOrderBookFactory`.

## Purpose

To maintain a collection of order books and route incoming `BookUpdate` events based on `SymbolId`.

## Interfaces

```cpp
class IOrderRouter {
public:
  virtual ~IOrderRouter() = default;
  virtual void route(const BookUpdate &update) = 0;
  virtual const IOrderBook *getBook(SymbolId id) const = 0;
};
```

```cpp
class SymbolIdOrderRouter : public IOrderRouter {
public:
  SymbolIdOrderRouter(SymbolRegistry *registry, IOrderBookFactory *factory);

  void registerBook(SymbolId id, const IOrderBookConfig &config);
  void route(const BookUpdate &update) override;
  const IOrderBook *getBook(SymbolId id) const override;
};
```

## Responsibilities

- `IOrderRouter` provides a generic API for routing and querying order books
- `SymbolIdOrderRouter`:
  - Manages one book per symbol ID
  - Uses `IOrderBookFactory` to instantiate order books
  - Routes book updates to the correct book by `SymbolId`

## Thread Safety

- Internally guarded by `std::mutex`
- Safe for concurrent access from multiple threads

## Use Cases

- Central routing of market data in multi-symbol systems
- Decouples book creation/config from data ingestion
- Enables modular integration with symbol-based trading strategies

## Notes

- Ownership of created books is internal
- `SymbolRegistry` is used only for symbol resolution, not required at runtime