/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/strategy/strategy_manager.h"
#include "flox/common.h"

namespace flox {

void StrategyManager::start() {
  for (auto &strategy : _strategies) {
    strategy->onStart();
  }
}
void StrategyManager::stop() {
  for (auto &strategy : _strategies) {
    strategy->onStop();
  }
}

void StrategyManager::addStrategy(const std::shared_ptr<IStrategy> &strategy) {
  if (_positionManager) {
    strategy->setPositionManager(_positionManager);
  }
  _strategies.push_back(strategy);
}

void StrategyManager::onCandle(SymbolId symbol, const Candle &candle) {
  for (auto &strategy : _strategies) {
    strategy->onCandle(symbol, candle);
  }
}

void StrategyManager::onTrade(const Trade &trade) {
  for (auto &strategy : _strategies) {
    strategy->onTrade(trade);
  }
}

void StrategyManager::onBookUpdate(const BookUpdate &bookUpdate) {
  for (auto &strategy : _strategies) {
    strategy->onBookUpdate(bookUpdate);
  }
}

} // namespace flox
