/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/book/candle.h"
#include "flox/book/trade.h"
#include "flox/common.h"
#include "flox/engine/engine.h"
#include "flox/engine/subsystem.h"

#include <chrono>
#include <functional>
#include <string>
#include <unordered_map>

namespace flox {

class CandleAggregator : public ISubsystem {
public:
  using CandleCallback = std::function<void(SymbolId, const Candle &)>;

  CandleAggregator(std::chrono::seconds interval, CandleCallback callback);

  void start() override;
  void stop() override;

  void onTrade(const Trade &trade);

private:
  struct PartialCandle {
    Candle candle;
    bool initialized = false;
  };

  std::chrono::seconds _interval;
  CandleCallback _callback;
  std::unordered_map<SymbolId, PartialCandle> _candles;

  std::chrono::system_clock::time_point
  alignToInterval(std::chrono::system_clock::time_point tp);
};

} // namespace flox
