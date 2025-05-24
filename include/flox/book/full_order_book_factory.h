/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/book/abstract_order_book_factory.h"
#include "flox/book/full_order_book.h"

namespace flox {

struct FullOrderBookConfig : public IOrderBookConfig {};

class FullOrderBookFactory : public IOrderBookFactory {
public:
  IOrderBook *create(const IOrderBookConfig &) override {
    auto book = std::make_unique<FullOrderBook>();
    IOrderBook *ptr = book.get();
    _owned.emplace_back(std::move(book));
    return ptr;
  }

private:
  std::vector<std::unique_ptr<IOrderBook>> _owned;
};

} // namespace flox