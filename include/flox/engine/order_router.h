#pragma once

#include "flox/book/abstract_order_book.h"
#include "flox/book/abstract_order_book_factory.h"
#include "flox/book/book_update.h"
#include "flox/common.h"

#include <iostream>
#include <mutex>
#include <optional>
#include <unordered_map>

namespace flox {

class IOrderRouter {
public:
  virtual ~IOrderRouter() = default;
  virtual void route(const BookUpdate &update) = 0;
  virtual const IOrderBook *getBook(SymbolId id) const = 0;
};

class SymbolIdOrderRouter : public IOrderRouter {
public:
  SymbolIdOrderRouter(SymbolRegistry *registry, IOrderBookFactory *factory);

  void registerBook(SymbolId id, const IOrderBookConfig &config);
  void route(const BookUpdate &update) override;
  const IOrderBook *getBook(SymbolId id) const override;

private:
  IOrderBookFactory *_orderBookFactory;
  SymbolRegistry *_registry;
  std::unordered_map<SymbolId, IOrderBook *> _books;
  mutable std::mutex _mutex;
};

} // namespace flox
