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
#include "flox/book/trade.h"

#include <functional>
#include <string>

namespace flox {

class ExchangeConnector {
public:
  virtual ~ExchangeConnector() = default;

  using BookUpdateCallback = std::function<void(const BookUpdate &)>;
  using TradeCallback = std::function<void(const Trade &)>;

  virtual void start() = 0;
  virtual void stop() = 0;

  virtual std::string exchangeId() const = 0;

  virtual void setCallbacks(BookUpdateCallback onBookUpdate,
                            TradeCallback onTrade) {
    _onBookUpdate = std::move(onBookUpdate);
    _onTrade = std::move(onTrade);
  }

protected:
  void emitBookUpdate(const BookUpdate &bu) {
    if (_onBookUpdate)
      _onBookUpdate(bu);
  }

  void emitTrade(const Trade &t) {
    if (_onTrade)
      _onTrade(t);
  }

private:
  BookUpdateCallback _onBookUpdate;
  TradeCallback _onTrade;
};

} // namespace flox
