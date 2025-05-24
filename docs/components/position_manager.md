# Position Manager

The `PositionManager` tracks and updates symbol-level positions in response to order fills and rejections.  
It also serves as an execution listener and integrates as a subsystem in the Flox framework.

## Responsibilities

- Maintains internal positions per symbol using fixed-point arithmetic
- Reacts to order fills and rejections via the `IOrderExecutionListener` interface
- Supports querying of position sizes for individual symbols
- Implements `ISubsystem` for lifecycle integration
- Optional component (not strictly required for engine operation)

## Class Definition

```cpp
class PositionManager : public IPositionManager,
                        public IOrderExecutionListener,
                        public ISubsystem {
public:
  void start() override;
  void stop() override;

  void onOrderFilled(const Order &order) override;
  void onOrderRejected(const Order &order, const std::string &reason) override;

  double getPosition(SymbolId symbol) const override;
  void printPositions() const;
};
```

## Internal Representation

- Positions are stored as integers with fixed precision (`QTY_PRECISION = 1,000,000`) for performance and consistency
- Uses a static vector of size `MAX_SYMBOLS = 65,536` indexed by `SymbolId`
- `toInternal(double qty)` and `toDisplay(int64_t qty)` are used to convert quantities

## Notes

- PositionManager is optional. If your system does not require tracking net positions, this component can be omitted.