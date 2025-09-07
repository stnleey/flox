/*
 * Flox Engine
 * Developed by FLOX Foundation (https://github.com/FLOX-Foundation)
 *
 * Copyright (c) 2025 FLOX Foundation
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include <string>
#include <vector>

namespace flox
{

struct SymbolConfig
{
  std::string symbol;
  double tickSize;
  double expectedDeviation;
};

struct ExchangeConfig
{
  std::string name;
  std::string type;
  std::vector<SymbolConfig> symbols;
};

struct KillSwitchConfig
{
  double maxOrderQty = 10'000.0;
  double maxLoss = -1e6;
  int maxOrdersPerSecond = -1;
};

struct EngineConfig
{
  std::vector<ExchangeConfig> exchanges;
  KillSwitchConfig killSwitchConfig;

  std::string logLevel = "info";
  std::string logFile;
};

#ifndef FLOX_DEFAULT_EVENTBUS_CAPACITY
#define FLOX_DEFAULT_EVENTBUS_CAPACITY 4096
#endif

#ifndef FLOX_DEFAULT_EVENTBUS_MAX_CONSUMERS
#define FLOX_DEFAULT_EVENTBUS_MAX_CONSUMERS 128
#endif

#ifndef FLOX_DEFAULT_ORDER_TRACKER_CAPACITY
#define FLOX_DEFAULT_ORDER_TRACKER_CAPACITY 4096
#endif

namespace config
{

inline constexpr size_t DEFAULT_EVENTBUS_CAPACITY = FLOX_DEFAULT_EVENTBUS_CAPACITY;
inline constexpr size_t DEFAULT_EVENTBUS_MAX_CONSUMERS = FLOX_DEFAULT_EVENTBUS_MAX_CONSUMERS;

// CPU Affinity Priority Constants
inline constexpr int ISOLATED_CORE_PRIORITY_BOOST = 5;
inline constexpr int DEFAULT_REALTIME_PRIORITY = 80;
inline constexpr int FALLBACK_REALTIME_PRIORITY = 90;

// Component-specific priority constants
inline constexpr int MARKET_DATA_PRIORITY = 90;
inline constexpr int EXECUTION_PRIORITY = 85;
inline constexpr int STRATEGY_PRIORITY = 80;
inline constexpr int RISK_PRIORITY = 75;
inline constexpr int GENERAL_PRIORITY = 70;

// Order tracker capacity
inline constexpr int ORDER_TRACKER_CAPACITY = FLOX_DEFAULT_ORDER_TRACKER_CAPACITY;

}  // namespace config
}  // namespace flox
