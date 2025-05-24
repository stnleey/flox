# Common Types

This file defines basic enums and type aliases used throughout the Flox framework.  
These primitives standardize order semantics and symbol representation.

## Enums

### `OrderType`

```cpp
enum class OrderType { LIMIT, MARKET };
```

- `LIMIT`: Order to be executed at a specific price or better
- `MARKET`: Order to be executed immediately at the best available price

### `Side`

```cpp
enum class Side { BUY, SELL };
```

- `BUY`: Indicates a buy-side order or trade
- `SELL`: Indicates a sell-side order or trade

## Type Aliases

### `SymbolId`

```cpp
using SymbolId = uint32_t;
```

- A compact, internal identifier used to reference trading symbols across all components.
- Typically mapped from exchange:symbol strings using `SymbolRegistry`.

## Use Cases

- Order routing and strategy logic
- Efficient mapping and lookup
- Readable and strongly-typed API