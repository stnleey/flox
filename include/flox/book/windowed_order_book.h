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
#include "flox/book/book_side.h"
#include "flox/book/book_update.h"

#include <memory_resource>
#include <mutex>
#include <optional>

namespace flox {

class WindowedOrderBook : public IOrderBook {
public:
  WindowedOrderBook(double tickSize, double expectedDeviation,
                    std::pmr::memory_resource *mem);

  void applyBookUpdate(const BookUpdate &update) override;

  size_t priceToIndex(double price) const;
  double indexToPrice(size_t index) const;
  bool isPriceInWindow(double price) const;

  double bidAtPrice(double price) const override;
  double askAtPrice(double price) const override;

  std::optional<double> bestBid() const override;
  std::optional<double> bestAsk() const override;

  double getBidQuantity(double price) const;
  double getAskQuantity(double price) const;

  void printBook(std::size_t depth = 10) const;

  double centerPrice() const { return _centerPrice; }

private:
  void shiftWindow(double newPrice);

  double _tickSize;
  double _invTickSize;
  std::size_t _windowSize;
  std::size_t _halfWindowSize;

  double _centerPrice;
  double _basePrice;

  BookSide _bids;
  BookSide _asks;

  mutable std::mutex _mutex;
};

} // namespace flox