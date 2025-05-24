/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/book/book_update_factory.h"
#include "flox/common.h"
#include "flox/strategy/strategy_manager.h"

#include <gtest/gtest.h>

using namespace flox;

class MockStrategy : public IStrategy {
public:
  int candles = 0;
  int trades = 0;
  int books = 0;

  void onCandle(SymbolId, const Candle &) override { ++candles; }
  void onTrade(const Trade &) override { ++trades; }
  void onBookUpdate(const BookUpdate &) override { ++books; }
};

TEST(StrategyManagerTest, InvokesAllStrategies) {
  StrategyManager manager;

  auto s1 = std::make_shared<MockStrategy>();
  auto s2 = std::make_shared<MockStrategy>();

  manager.addStrategy(s1);
  manager.addStrategy(s2);

  Candle c;
  Trade t;

  BookUpdateFactory factory;

  manager.onCandle(42, c);
  manager.onTrade(t);
  manager.onBookUpdate(factory.create());

  EXPECT_EQ(s1->candles, 1);
  EXPECT_EQ(s2->candles, 1);
  EXPECT_EQ(s1->trades, 1);
  EXPECT_EQ(s2->trades, 1);
  EXPECT_EQ(s1->books, 1);
  EXPECT_EQ(s2->books, 1);
}
