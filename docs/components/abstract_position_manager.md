# Position Manager Interface

The `IPositionManager` interface provides access to position information per symbol.  
It is typically implemented by a component that tracks fills and updates internal position state.

## Purpose

To allow strategies, trackers, or execution components to query current exposure levels per trading symbol.

## Interface Definition

```cpp
class IPositionManager {
public:
  virtual ~IPositionManager() = default;

  virtual double getPosition(SymbolId symbol) const = 0;
};
```

## Responsibilities

- `getPosition(...)`: returns the current net position for the specified symbol
- Underlying implementation must keep this data consistent and accurate

## Use Cases

- Risk checks before submitting new orders
- Displaying strategy exposure
- Calculating PnL or exposure-weighted metrics

## Notes

- This interface only provides read access
- Actual position tracking is done by components like `PositionManager`