/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/engine/order_router.h"

namespace flox {

SymbolIdOrderRouter::SymbolIdOrderRouter(SymbolRegistry *registry,
                                         IOrderBookFactory *factory)
    : _orderBookFactory(factory), _registry(registry) {}

void SymbolIdOrderRouter::registerBook(SymbolId id,
                                       const IOrderBookConfig &config) {
  std::scoped_lock lock(_mutex);
  if (_books.contains(id)) {
    std::cerr << "[OrderRouter] Duplicate SymbolId: " << id << "\n";
    return;
  }

  IOrderBook *book = _orderBookFactory->create(config);
  _books.emplace(id, book);
}

void SymbolIdOrderRouter::route(const BookUpdate &update) {
  std::scoped_lock lock(_mutex);

  auto it = _books.find(update.symbol);
  if (it != _books.end()) {
    it->second->applyBookUpdate(update);
  } else {
    std::cerr << "[OrderRouter] Book not registered for SymbolId "
              << update.symbol << "\n";
  }
}

const IOrderBook *SymbolIdOrderRouter::getBook(SymbolId id) const {
  std::scoped_lock lock(_mutex);
  auto it = _books.find(id);
  if (it != _books.end())
    return it->second;
  return nullptr;
}

} // namespace flox
