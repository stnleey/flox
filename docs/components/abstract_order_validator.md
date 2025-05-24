# Order Validator

The `IOrderValidator` interface defines a contract for validating orders before submission.  
It is intended to enforce basic or complex sanity checks to prevent erroneous or harmful trades.

## Purpose

To ensure that orders meet criteria such as:
- Minimum quantity
- Price range limits
- Symbol-specific constraints

## Interface Definition

```cpp
class IOrderValidator {
public:
  virtual ~IOrderValidator() = default;

  virtual bool validate(const Order &order, std::string &reason) const = 0;
};
```

## Responsibilities

- `validate(...)`: inspects the order and returns `true` if valid.  
  If `false`, the `reason` string is populated with the validation failure message.

## Use Cases

- Plugged into the order pipeline before execution
- Risk-limiting logic for rogue order prevention
- Strategy-specific restrictions (e.g., price bands, notional value limits)

## Notes

- Validation does not modify the order
- Stateless implementations are preferred for thread safety