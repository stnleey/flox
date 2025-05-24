/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/common.h"
#include "flox/engine/engine.h"
#include "flox/engine/subsystem.h"
#include "flox/strategy/abstract_strategy.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace flox {

class StrategyManager : public ISubsystem {
public:
  void start() override;
  void stop() override;

  void setPositionManager(IPositionManager *pm) { _positionManager = pm; }
  void addStrategy(const std::shared_ptr<IStrategy> &strategy);

  void onCandle(SymbolId symbol, const Candle &candle);
  void onTrade(const Trade &trade);
  void onBookUpdate(const BookUpdate &bookUpdate);

private:
  std::vector<std::shared_ptr<IStrategy>> _strategies;
  IPositionManager *_positionManager = nullptr;
};

} // namespace flox
