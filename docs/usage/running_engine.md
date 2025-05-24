# Running the Flox Engine

This guide explains how to initialize and run the Flox engine using a custom `IEngineBuilder` implementation.

## Structure

To launch the engine, you are expected to:

1. Implement the `IEngineBuilder` interface
2. Provide subsystems, connectors, and strategies
3. Return a fully initialized `Engine` instance

## Example

Here’s a simplified version based on `DemoEngineBuilder`:

```cpp
class DemoEngineBuilder : public IEngineBuilder {
public:
  DemoEngineBuilder(const EngineConfig &config)
      : _config(config) {}

  std::unique_ptr<Engine> build() override {
    auto registry = std::make_unique<SymbolRegistry>();
    auto mdb = std::make_unique<MarketDataBus>();

    // Register exchange connectors
    ConnectorFactory::instance().registerConnector("bybit",
      [mdb = mdb.get(), registry = registry.get()](const std::string &symbolStr) {
        auto symbolId = registry->getSymbolId("bybit", symbolStr);
        auto conn = std::make_shared<BybitExchangeConnector>(symbolStr, *symbolId);
        conn->setCallbacks(
          [mdb](const BookUpdate &b) { mdb->onBookUpdate(b); },
          [mdb](const Trade &t) { mdb->onTrade(t); });
        return conn;
      });

    std::vector<std::shared_ptr<ExchangeConnector>> connectors;
    std::vector<std::unique_ptr<ISubsystem>> subsystems;

    // Register symbols and connectors
    for (const auto &ex : _config.exchanges) {
      for (const auto &sym : ex.symbols) {
        auto id = registry->registerSymbol(ex.name, sym.symbol);
        auto conn = ConnectorFactory::instance().createConnector(ex.name, sym.symbol);
        if (conn) connectors.push_back(conn);
      }
    }

    auto strategyMgr = std::make_unique<StrategyManager>();

    // Subscribe strategies to market data
    for (const auto &symbol : registry->getAllSymbolIds()) {
      mdb->subscribeToBookUpdates(symbol, [strategyMgrRaw = strategyMgr.get()](const BookUpdate &b) {
        strategyMgrRaw->onBookUpdate(b);
      });
      mdb->subscribeToTrades(symbol, [strategyMgrRaw = strategyMgr.get()](const Trade &t) {
        strategyMgrRaw->onTrade(t);
      });
    }

    // Load strategies from config
    auto strategies = ImpulseBreakoutStrategyFactory::createStrategiesFromFile(
      "impulse_breakout_config.json", registry.get());

    for (const auto &strat : strategies) {
      auto executor = std::make_unique<SimulatedOrderExecutor>();
      strat->setOrderExecutor(executor.get());
      strategyMgr->addStrategy(strat);
      subsystems.push_back(std::move(executor));
    }

    subsystems.push_back(std::move(mdb));
    subsystems.push_back(std::move(strategyMgr));
    subsystems.push_back(std::make_unique<Subsystem<SymbolRegistry>>(std::move(registry)));

    return std::make_unique<Engine>(_config, std::move(subsystems), std::move(connectors));
  }

private:
  EngineConfig _config;
};
```

## Notes

- Use `Subsystem<T>` to wrap all non-ISubsystem components
- Strategy implementations and symbol registration should be dynamic
- Execution is triggered via:

```cpp
auto engine = builder.build();
engine->start();
```

Refer to your implementation of `EngineBuilder` to control strategy loading and exchange support.