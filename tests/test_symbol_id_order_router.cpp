/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/book/abstract_order_book_factory.h"
#include "flox/book/book_update_factory.h"
#include "flox/engine/order_router.h"
#include "flox/engine/symbol_registry.h"

#include <gtest/gtest.h>

using namespace flox;

namespace {

class MockOrderBook : public IOrderBook {
public:
  void applyBookUpdate(const BookUpdate &update) override { _updated = true; }

  std::optional<double> bestBid() const override { return std::nullopt; }
  std::optional<double> bestAsk() const override { return std::nullopt; }

  double bidAtPrice(double price) const override { return {}; };
  double askAtPrice(double price) const override { return {}; };

  bool wasUpdated() const { return _updated; }

private:
  bool _updated = false;
};

class MockOrderBookConfig : public IOrderBookConfig {};

class MockOrderBookFactory : public IOrderBookFactory {
public:
  IOrderBook *create(const IOrderBookConfig &config) {
    auto book = std::make_unique<MockOrderBook>();
    IOrderBook *ptr = book.get();
    _owned.emplace_back(std::move(book));
    return ptr;
  }

  const std::vector<std::unique_ptr<MockOrderBook>> &books() const {
    return _owned;
  }

private:
  std::vector<std::unique_ptr<MockOrderBook>> _owned;
};

} // namespace

class SymbolIdOrderRouterTest : public ::testing::Test {
protected:
  SymbolRegistry registry;
  MockOrderBookFactory factory;
  SymbolIdOrderRouter router{&registry, &factory};
  BookUpdateFactory updateFactory;

  void SetUp() override {}
};

TEST_F(SymbolIdOrderRouterTest, RegistersAndRetrievesBook) {
  SymbolId symbol = registry.registerSymbol("bybit", "BTCUSDT");
  router.registerBook(symbol, MockOrderBookConfig{});

  const auto *book = router.getBook(symbol);
  ASSERT_NE(book, nullptr);
}

TEST_F(SymbolIdOrderRouterTest, DuplicateRegistrationLogsError) {
  SymbolId symbol = registry.registerSymbol("bybit", "BTCUSDT");

  testing::internal::CaptureStderr();
  router.registerBook(symbol, MockOrderBookConfig{});
  router.registerBook(symbol, MockOrderBookConfig{});
  std::string output = testing::internal::GetCapturedStderr();

  EXPECT_NE(output.find("Duplicate SymbolId"), std::string::npos);
}

TEST_F(SymbolIdOrderRouterTest, AppliesBookUpdate) {
  SymbolId symbol = registry.registerSymbol("bybit", "ETHUSDT");
  router.registerBook(symbol, MockOrderBookConfig{});

  auto update = updateFactory.create();
  update.symbol = symbol;
  update.type = BookUpdateType::SNAPSHOT;
  update.bids = {{1000.0, 2.0}};
  update.asks = {{1001.0, 1.5}};
  router.route(update);

  auto *book = dynamic_cast<const MockOrderBook *>(router.getBook(symbol));
  ASSERT_NE(book, nullptr);
  EXPECT_TRUE(book->wasUpdated());
}

TEST_F(SymbolIdOrderRouterTest, LogsMissingBook) {
  auto update = updateFactory.create();
  update.symbol = 9999;
  update.type = BookUpdateType::SNAPSHOT;

  testing::internal::CaptureStderr();
  router.route(update);
  std::string output = testing::internal::GetCapturedStderr();

  EXPECT_NE(output.find("Book not registered for SymbolId"), std::string::npos);
}
