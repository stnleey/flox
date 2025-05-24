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
#include "flox/engine/subsystem.h"
#include <functional>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <vector>

namespace flox {

class MarketDataBus : public ISubsystem {
public:
  using CandleCallback = std::function<void(SymbolId, const Candle &)>;
  using TradeCallback = std::function<void(const Trade &)>;
  using BookUpdateCallback = std::function<void(const BookUpdate &)>;

  enum class SubscriptionType { Candle, Trade, BookUpdate };

  struct SubscriptionHandle {
    SymbolId symbol;
    SubscriptionType type;
    size_t index;

    bool operator==(const SubscriptionHandle &other) const {
      return symbol == other.symbol && type == other.type &&
             index == other.index;
    }
  };

  SubscriptionHandle subscribeToCandles(SymbolId symbol, CandleCallback cb);
  SubscriptionHandle subscribeToTrades(SymbolId symbol, TradeCallback cb);
  SubscriptionHandle subscribeToBookUpdates(SymbolId symbol,
                                            BookUpdateCallback cb);

  void unsubscribe(const SubscriptionHandle &handle);

  void onCandle(SymbolId symbol, const Candle &candle);
  void onTrade(const Trade &trade);
  void onBookUpdate(const BookUpdate &update);

  void clear();

  void start() override;
  void stop() override;

private:
  struct Router {
    std::vector<std::optional<CandleCallback>> candleSubs;
    std::vector<std::optional<TradeCallback>> tradeSubs;
    std::vector<std::optional<BookUpdateCallback>> bookSubs;
  };

  std::unordered_map<SymbolId, Router> _routers;
  std::mutex _mutex;
};

} // namespace flox
