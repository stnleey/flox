/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/book/book_update.h"
#include "flox/book/book_update_factory.h"
#include "flox/book/candle.h"
#include "flox/book/trade.h"
#include "flox/common.h"
#include "flox/engine/market_data_bus.h"

#include <gtest/gtest.h>
#include <string>
#include <thread>

using namespace flox;

constexpr SymbolId SYMBOL = 1;

// Verifies that candle subscribers are notified with correct symbol and price
TEST(MarketDataBus, CandleCallbackIsCalled) {
  MarketDataBus bus;
  bool called = false;

  bus.subscribeToCandles(SYMBOL, [&](SymbolId symbol, const Candle &candle) {
    called = true;
    EXPECT_EQ(symbol, SYMBOL);
    EXPECT_DOUBLE_EQ(candle.close, 6.66);
  });

  Candle candle;
  candle.close = 6.66;
  bus.onCandle(SYMBOL, candle);

  EXPECT_TRUE(called);
}

// Verifies that trade subscribers receive trade data correctly
TEST(MarketDataBus, TradeCallbackIsCalled) {
  MarketDataBus bus;
  bool called = false;

  bus.subscribeToTrades(SYMBOL, [&](const Trade &trade) {
    called = true;
    EXPECT_EQ(trade.symbol, SYMBOL);
    EXPECT_DOUBLE_EQ(trade.price, 5.55);
  });

  Trade trade;
  trade.symbol = SYMBOL;
  trade.price = 5.55;
  bus.onTrade(trade);

  EXPECT_TRUE(called);
}

// Verifies that book update subscribers are called with correct update type
TEST(MarketDataBus, BookUpdateCallbackIsCalled) {
  MarketDataBus bus;
  bool called = false;

  bus.subscribeToBookUpdates(SYMBOL, [&](const BookUpdate &update) {
    called = true;
    EXPECT_EQ(update.symbol, SYMBOL);
    EXPECT_EQ(update.type, BookUpdateType::DELTA);
  });

  BookUpdateFactory factory;
  auto update = factory.create();
  update.type = BookUpdateType::DELTA;
  update.symbol = SYMBOL;
  bus.onBookUpdate(update);

  EXPECT_TRUE(called);
}

// Verifies that all subscribers are called (not just the first one)
TEST(MarketDataBus, MultipleSubscribersAreCalled) {
  MarketDataBus bus;
  int candleCount = 0;
  int tradeCount = 0;
  int bookCount = 0;

  bus.subscribeToCandles(SYMBOL,
                         [&](SymbolId, const Candle &) { ++candleCount; });
  bus.subscribeToCandles(SYMBOL,
                         [&](SymbolId, const Candle &) { ++candleCount; });

  bus.subscribeToTrades(SYMBOL, [&](const Trade &) { ++tradeCount; });
  bus.subscribeToTrades(SYMBOL, [&](const Trade &) { ++tradeCount; });

  bus.subscribeToBookUpdates(SYMBOL, [&](const BookUpdate &) { ++bookCount; });
  bus.subscribeToBookUpdates(SYMBOL, [&](const BookUpdate &) { ++bookCount; });

  BookUpdateFactory factory;
  auto update = factory.create();
  update.symbol = SYMBOL;

  bus.onCandle(SYMBOL, Candle{});
  bus.onTrade(Trade{.symbol = SYMBOL});
  bus.onBookUpdate(update);

  EXPECT_EQ(candleCount, 2);
  EXPECT_EQ(tradeCount, 2);
  EXPECT_EQ(bookCount, 2);
}

// Verifies concurrent subscription and publishing does not crash and all
// callbacks are called
TEST(MarketDataBus, ConcurrentSubscriptionAndPublish) {
  MarketDataBus bus;

  std::atomic<int> candleCount{0};
  std::atomic<int> tradeCount{0};
  std::atomic<int> bookCount{0};

  constexpr int numSubscribers = 10;
  constexpr int numPublishers = 10;
  constexpr int numEventsPerPublisher = 100;

  // Spawn subscriber threads
  std::vector<std::thread> subscriberThreads;
  for (int i = 0; i < numSubscribers; ++i) {
    subscriberThreads.emplace_back([&]() {
      bus.subscribeToCandles(SYMBOL, [&](SymbolId, const Candle &) {
        candleCount.fetch_add(1, std::memory_order_relaxed);
      });
      bus.subscribeToTrades(SYMBOL, [&](const Trade &) {
        tradeCount.fetch_add(1, std::memory_order_relaxed);
      });
      bus.subscribeToBookUpdates(SYMBOL, [&](const BookUpdate &) {
        bookCount.fetch_add(1, std::memory_order_relaxed);
      });
    });
  }

  for (auto &t : subscriberThreads)
    t.join();

  // Spawn publisher threads
  std::vector<std::thread> publisherThreads;
  for (int i = 0; i < numPublishers; ++i) {
    publisherThreads.emplace_back([&]() {
      BookUpdateFactory factory;
      for (int j = 0; j < numEventsPerPublisher; ++j) {
        bus.onCandle(SYMBOL, Candle{});
        bus.onTrade(Trade{.symbol = SYMBOL});
        bus.onBookUpdate(factory.create());
      }
    });
  }

  for (auto &t : publisherThreads)
    t.join();

  int expectedCallbacks =
      numSubscribers * numPublishers * numEventsPerPublisher;

  EXPECT_EQ(candleCount.load(), expectedCallbacks);
  EXPECT_EQ(tradeCount.load(), expectedCallbacks);
  EXPECT_EQ(bookCount.load(), expectedCallbacks);
}

// Verifies that unsubscribe disables a specific callback
TEST(MarketDataBus, UnsubscribeDisablesCallback) {
  MarketDataBus bus;
  bool called = false;

  auto handle =
      bus.subscribeToTrades(SYMBOL, [&](const Trade &) { called = true; });

  bus.unsubscribe(handle);
  bus.onTrade(Trade{.symbol = SYMBOL});

  EXPECT_FALSE(called);
}

// Verifies that a callback can be re-subscribed after being unsubscribed
TEST(MarketDataBus, ReSubscribeAfterUnsubscribe) {
  MarketDataBus bus;
  bool called = false;

  auto handle =
      bus.subscribeToTrades(SYMBOL, [&](const Trade &) { called = true; });

  bus.unsubscribe(handle);

  handle = bus.subscribeToTrades(SYMBOL, [&](const Trade &) { called = true; });

  bus.onTrade(Trade{.symbol = SYMBOL});

  EXPECT_TRUE(called);
}