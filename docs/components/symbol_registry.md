# SymbolRegistry

The `SymbolRegistry` provides a compact mapping between string-based exchange:symbol names and internal `SymbolId` integers.  
It supports efficient registration, lookup, and reverse resolution of trading symbols.

## Purpose

To assign and maintain stable, compact `SymbolId` values for all exchange-symbol pairs.

## Class Definition

```cpp
class SymbolRegistry {
public:
  SymbolId registerSymbol(const std::string &exchange,
                          const std::string &symbol);

  std::optional<SymbolId> getSymbolId(const std::string &exchange,
                                      const std::string &symbol) const;

  std::pair<std::string, std::string> getSymbolName(SymbolId id) const;
};
```

## Responsibilities

- Registers symbols and assigns a unique `SymbolId` per `(exchange, symbol)` pair
- Supports reverse lookup of exchange/symbol strings by ID
- Ensures thread-safe access via `std::mutex`

## Use Cases

- Central registry used across the engine for symbol resolution
- Used by routers, strategies, order books, and logging
- Enables fast ID-based access instead of string comparisons

## Internal Design

- `_map`: maps `"exchange:symbol"` to `SymbolId`
- `_reverse`: vector of pairs for reverse mapping by index
- `_mutex`: protects access to shared state

## Notes

- Once registered, symbol IDs remain stable
- Registry must be prepopulated at engine startup or config loading phase