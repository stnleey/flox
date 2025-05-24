# ConnectorFactory

The `ConnectorFactory` is a registry for dynamically constructing `ExchangeConnector` instances by type.  
It supports runtime registration of connector implementations and symbol-specific instantiation.

## Purpose

To decouple connector instantiation from hardcoded logic and enable flexible, pluggable exchange integration.

## Class Definition

```cpp
class ConnectorFactory {
public:
  using CreatorFunc = std::function<std::shared_ptr<ExchangeConnector>(const std::string &symbol)>;

  static ConnectorFactory &instance();

  void registerConnector(const std::string &type, CreatorFunc creator);
  std::shared_ptr<ExchangeConnector> createConnector(const std::string &type, const std::string &symbol) const;

private:
  ConnectorFactory() = default;
};
```

## Responsibilities

- Registers connector creation functions keyed by string type (e.g. `"bybit"`, `"mock"`)
- Creates new `ExchangeConnector` instances on demand using the symbol name
- Acts as a singleton factory used during engine startup or reconfiguration

## Use Cases

- Symbol-specific connector routing
- Plugin-like architecture for exchange connectors
- Dynamic or test-time substitution of connector logic

## Notes

- The `CreatorFunc` lambda should capture and bind any necessary context
- Returns `nullptr` if the requested type is not registered