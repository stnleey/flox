# EngineConfig

The `EngineConfig` struct defines the top-level configuration for the Flox engine.  
It includes exchange setups, risk parameters, and logging preferences.

## Purpose

To provide a unified and structured way to configure system behavior and enabled components at startup.

## Struct Definitions

```cpp
struct SymbolConfig {
  std::string symbol;
  double tickSize;
  double expectedDeviation;
};

struct ExchangeConfig {
  std::string name;
  std::string type;
  std::vector<SymbolConfig> symbols;
};

struct KillSwitchConfig {
  double maxOrderQty = 10'000.0;
  double maxLoss = -1e6;
  int maxOrdersPerSecond = -1;
};

struct EngineConfig {
  std::vector<ExchangeConfig> exchanges;
  KillSwitchConfig killSwitchConfig;

  std::string logLevel = "info";
  std::string logFile;
};
```

## Configuration Fields

### Exchanges

- Each `ExchangeConfig` describes one exchange to connect:
  - `name`: logical identifier
  - `type`: connector type (e.g. `"mock"`, `"bybit"`)
  - `symbols`: list of `SymbolConfig` describing symbols and tick parameters

### KillSwitchConfig

- `maxOrderQty`: hard limit on order size
- `maxLoss`: max PnL drawdown before triggering kill switch
- `maxOrdersPerSecond`: order rate limit (disabled if < 0)

### Logging

- `logLevel`: desired logging level (e.g., `"info"`, `"debug"`)
- `logFile`: path to output log file

## Use Cases

- Loaded during engine initialization
- Passed to components like the builder, kill switch, and symbol registry
- Used to control runtime constraints and outputs

## Notes

- Engine assumes config is preloaded from JSON or other formats
- Supports both production and testing environments