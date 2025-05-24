/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/book/book_update.h"
#include "flox/book/candle.h"
#include "flox/book/trade.h"
#include "flox/common.h"
#include "flox/execution/abstract_execution_listener.h"
#include "flox/execution/abstract_executor.h"
#include "flox/position/abstract_position_manager.h"
#include "flox/risk/abstract_risk_manager.h"
#include "flox/validation/abstract_order_validator.h"

#include <string>

namespace flox {

class IStrategy {
public:
  virtual ~IStrategy() = default;

  virtual void onStart() {}
  virtual void onStop() {}

  virtual void onCandle(SymbolId symbol, const Candle &candle){};
  virtual void onTrade(const Trade &trade){};
  virtual void onBookUpdate(const BookUpdate &bookUpdate){};

  void setPositionManager(IPositionManager *manager) {
    _positionManager = manager;
  }
  void setRiskManager(IRiskManager *manager) { _riskManager = manager; }
  void setOrderExecutor(IOrderExecutor *executor) { _executor = executor; }
  void setOrderValidator(IOrderValidator *validator) { _validator = validator; }

protected:
  IRiskManager *GetRiskManager() { return _riskManager; }
  IPositionManager *GetPositionManager() { return _positionManager; }
  IOrderExecutor *GetOrderExecutor() { return _executor; }
  IOrderValidator *GetOrderValidator() { return _validator; }

private:
  IRiskManager *_riskManager = nullptr;
  IPositionManager *_positionManager = nullptr;
  IOrderExecutor *_executor = nullptr;
  IOrderValidator *_validator = nullptr;
};

} // namespace flox
