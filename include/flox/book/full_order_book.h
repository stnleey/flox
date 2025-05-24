/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/book/abstract_order_book.h"
#include "flox/book/book_update.h"

#include <map>
#include <mutex>
#include <optional>

namespace flox {

class FullOrderBook : public IOrderBook {
public:
  void applyBookUpdate(const BookUpdate &update) override;
  std::optional<double> bestBid() const override;
  std::optional<double> bestAsk() const override;

  double bidAtPrice(double price) const override;
  double askAtPrice(double price) const override;

private:
  mutable std::mutex _mutex;
  std::map<double, double, std::greater<>> _bids;
  std::map<double, double> _asks;
};

} // namespace flox
