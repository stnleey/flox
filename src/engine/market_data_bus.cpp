/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/engine/market_data_bus.h"

namespace flox {

MarketDataBus::SubscriptionHandle
MarketDataBus::subscribeToCandles(SymbolId symbol, CandleCallback cb) {
  std::scoped_lock lock(_mutex);
  auto &router = _routers[symbol];
  router.candleSubs.emplace_back(std::move(cb));
  return {symbol, SubscriptionType::Candle, router.candleSubs.size() - 1};
}

MarketDataBus::SubscriptionHandle
MarketDataBus::subscribeToTrades(SymbolId symbol, TradeCallback cb) {
  std::scoped_lock lock(_mutex);
  auto &router = _routers[symbol];
  router.tradeSubs.emplace_back(std::move(cb));
  return {symbol, SubscriptionType::Trade, router.tradeSubs.size() - 1};
}

MarketDataBus::SubscriptionHandle
MarketDataBus::subscribeToBookUpdates(SymbolId symbol, BookUpdateCallback cb) {
  std::scoped_lock lock(_mutex);
  auto &router = _routers[symbol];
  router.bookSubs.emplace_back(std::move(cb));
  return {symbol, SubscriptionType::BookUpdate, router.bookSubs.size() - 1};
}

void MarketDataBus::unsubscribe(const SubscriptionHandle &handle) {
  std::scoped_lock lock(_mutex);
  auto it = _routers.find(handle.symbol);
  if (it == _routers.end())
    return;

  auto &router = it->second;
  switch (handle.type) {
  case SubscriptionType::Candle:
    if (handle.index < router.candleSubs.size()) {
      router.candleSubs[handle.index] = std::nullopt;
    }
    break;
  case SubscriptionType::Trade:
    if (handle.index < router.tradeSubs.size()) {
      router.tradeSubs[handle.index] = std::nullopt;
    }
    break;
  case SubscriptionType::BookUpdate:
    if (handle.index < router.bookSubs.size()) {
      router.bookSubs[handle.index] = std::nullopt;
    }
    break;
  }
}

void MarketDataBus::onCandle(SymbolId symbol, const Candle &candle) {
  std::vector<CandleCallback> subs;
  {
    std::scoped_lock lock(_mutex);
    auto it = _routers.find(symbol);
    if (it == _routers.end())
      return;
    for (const auto &cb : it->second.candleSubs) {
      if (cb)
        subs.push_back(*cb);
    }
  }

  for (const auto &cb : subs)
    cb(symbol, candle);
}

void MarketDataBus::onTrade(const Trade &trade) {
  std::vector<TradeCallback> subs;
  {
    std::scoped_lock lock(_mutex);
    auto it = _routers.find(trade.symbol);
    if (it == _routers.end())
      return;
    for (const auto &cb : it->second.tradeSubs) {
      if (cb)
        subs.push_back(*cb);
    }
  }

  for (const auto &cb : subs)
    cb(trade);
}

void MarketDataBus::onBookUpdate(const BookUpdate &update) {
  std::vector<BookUpdateCallback> subs;
  {
    std::scoped_lock lock(_mutex);
    auto it = _routers.find(update.symbol);
    if (it == _routers.end())
      return;
    for (const auto &cb : it->second.bookSubs) {
      if (cb)
        subs.push_back(*cb);
    }
  }

  for (const auto &cb : subs)
    cb(update);
}

void MarketDataBus::clear() {
  std::scoped_lock lock(_mutex);
  _routers.clear();
}

void MarketDataBus::start() {}
void MarketDataBus::stop() {}

} // namespace flox
