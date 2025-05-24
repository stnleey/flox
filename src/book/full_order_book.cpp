/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/book/full_order_book.h"

namespace flox {

void FullOrderBook::applyBookUpdate(const BookUpdate &update) {
  std::scoped_lock lock(_mutex);

  if (update.type == BookUpdateType::SNAPSHOT) {
    _bids.clear();
    _asks.clear();
  }

  for (const auto &[price, qty] : update.bids) {
    if (qty == 0.0)
      _bids.erase(price);
    else
      _bids[price] = qty;
  }

  for (const auto &[price, qty] : update.asks) {
    if (qty == 0.0)
      _asks.erase(price);
    else
      _asks[price] = qty;
  }
}

std::optional<double> FullOrderBook::bestBid() const {
  std::scoped_lock lock(_mutex);
  if (_bids.empty())
    return std::nullopt;
  return _bids.begin()->first;
}

std::optional<double> FullOrderBook::bestAsk() const {
  std::scoped_lock lock(_mutex);
  if (_asks.empty())
    return std::nullopt;
  return _asks.begin()->first;
}

double FullOrderBook::bidAtPrice(double price) const {
  std::scoped_lock lock(_mutex);
  auto it = _bids.find(price);
  return it != _bids.end() ? it->second : 0.0;
}

double FullOrderBook::askAtPrice(double price) const {
  std::scoped_lock lock(_mutex);
  auto it = _asks.find(price);
  return it != _asks.end() ? it->second : 0.0;
}

} // namespace flox
