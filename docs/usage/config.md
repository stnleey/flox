# Configuration

Flox is configured via the `EngineConfig` structure, typically loaded from a file.

## Example (JSON-like)

```json
{
  "logLevel": "debug",
  "exchanges": [
    {
      "name": "bybit",
      "type": "mock",
      "symbols": [
        { "symbol": "DOTUSDT", "tickSize": 0.001, "expectedDeviation": 0.5 }
      ]
    }
  ],
  "killSwitchConfig": {
    "maxOrderQty": 10000,
    "maxLoss": -5000,
    "maxOrdersPerSecond": 100
  }
}
```