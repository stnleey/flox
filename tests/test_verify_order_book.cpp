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
#include "flox/book/windowed_order_book.h"
#include "flox/book/windowed_order_book_factory.h"

#include <cmath>
#include <gtest/gtest.h>

using namespace flox;

TEST(OrderBookTest, SnapshotFitsInWindow) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 100.0; // window = 200 ticks

  WindowedOrderBookFactory orderBookFactory;
  BookUpdateFactory bookUpdateFactory;

  auto book =
      orderBookFactory.create(WindowedOrderBookConfig{tickSize, deviation});

  auto snapshot = bookUpdateFactory.create();
  snapshot.type = BookUpdateType::SNAPSHOT;
  snapshot.bids = {{100.0, 5.0}, {99.0, 3.0}};
  snapshot.asks = {{101.0, 2.0}, {102.0, 4.0}};

  book->applyBookUpdate(snapshot);

  EXPECT_DOUBLE_EQ(book->bidAtPrice(100.0), 5.0);
  EXPECT_DOUBLE_EQ(book->bidAtPrice(99.0), 3.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(101.0), 2.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(102.0), 4.0);
}

TEST(OrderBookTest, SnapshotPartiallyOutsideWindow) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 5.0; // window = 10 ticks

  WindowedOrderBookFactory orderBookFactory;
  BookUpdateFactory bookUpdateFactory;

  auto book =
      orderBookFactory.create(WindowedOrderBookConfig{tickSize, deviation});

  auto snapshot = bookUpdateFactory.create();
  snapshot.type = BookUpdateType::SNAPSHOT;

  for (double p = 200.0; p <= 210.0; ++p)
    snapshot.bids.push_back({p, 1.0});
  for (double p = 211.0; p <= 220.0; ++p)
    snapshot.asks.push_back({p, 1.0});

  book->applyBookUpdate(snapshot);

  for (double p = 200.0; p <= 210.0; ++p)
    EXPECT_GE(book->bidAtPrice(p), 0.0);
  for (double p = 211.0; p <= 220.0; ++p)
    EXPECT_GE(book->askAtPrice(p), 0.0);
}

TEST(OrderBookTest, DeltaWithinWindow) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 5.0;

  WindowedOrderBookFactory orderBookFactory;
  BookUpdateFactory bookUpdateFactory;
  auto book =
      orderBookFactory.create(WindowedOrderBookConfig{tickSize, deviation});

  auto snapshot = bookUpdateFactory.create();
  snapshot.type = BookUpdateType::SNAPSHOT;
  snapshot.bids = {{100.0, 1.0}};
  snapshot.asks = {{101.0, 1.0}};
  book->applyBookUpdate(snapshot);

  auto delta = bookUpdateFactory.create();
  delta.type = BookUpdateType::DELTA;
  delta.bids = {{99.0, 2.0}};
  delta.asks = {{102.0, 2.5}};
  book->applyBookUpdate(delta);

  EXPECT_DOUBLE_EQ(book->bidAtPrice(99.0), 2.0);
  EXPECT_DOUBLE_EQ(book->bidAtPrice(100.0), 1.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(101.0), 1.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(102.0), 2.5);
}

TEST(OrderBookTest, DeltaAtWindowEdge) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 5.0;

  WindowedOrderBookFactory orderBookFactory;
  BookUpdateFactory bookUpdateFactory;
  auto book =
      orderBookFactory.create(WindowedOrderBookConfig{tickSize, deviation});

  auto snapshot = bookUpdateFactory.create();
  snapshot.type = BookUpdateType::SNAPSHOT;
  snapshot.bids = {{100.0, 1.0}};
  snapshot.asks = {{101.0, 1.0}};
  book->applyBookUpdate(snapshot);

  double base = std::round((100.5 - 5.0) / tickSize) * tickSize;

  auto delta = bookUpdateFactory.create();
  delta.type = BookUpdateType::DELTA;
  delta.bids = {{base, 3.0}};
  delta.asks = {{base + 9.0 * tickSize, 3.0}};
  book->applyBookUpdate(delta);

  EXPECT_DOUBLE_EQ(book->bidAtPrice(100.0), 1.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(101.0), 1.0);
  EXPECT_DOUBLE_EQ(book->bidAtPrice(base), 3.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(base + 9.0), 3.0);
}

TEST(OrderBookTest, DeltaOutsideWindowIsIgnored) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 5.0;

  WindowedOrderBookFactory orderBookFactory;
  BookUpdateFactory bookUpdateFactory;
  auto book =
      orderBookFactory.create(WindowedOrderBookConfig{tickSize, deviation});

  auto snapshot = bookUpdateFactory.create();
  snapshot.type = BookUpdateType::SNAPSHOT;
  snapshot.bids = {{100.0, 1.0}};
  snapshot.asks = {{101.0, 1.0}};
  book->applyBookUpdate(snapshot);

  auto delta = bookUpdateFactory.create();
  delta.type = BookUpdateType::DELTA;
  delta.bids = {{80.0, 10.0}};
  delta.asks = {{120.0, 10.0}};
  book->applyBookUpdate(delta);

  EXPECT_DOUBLE_EQ(book->bidAtPrice(100.0), 1.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(101.0), 1.0);
  EXPECT_DOUBLE_EQ(book->bidAtPrice(80.0), 0.0);
  EXPECT_DOUBLE_EQ(book->askAtPrice(120.0), 0.0);
}

TEST(OrderBookTest, ShiftWindowCleansOldLevels) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 5.0;
  WindowedOrderBookFactory factory;
  BookUpdateFactory updateFactory;
  auto book = factory.create(WindowedOrderBookConfig{tickSize, deviation});

  auto snapshot1 = updateFactory.create();
  snapshot1.type = BookUpdateType::SNAPSHOT;
  snapshot1.bids = {{100.0, 5.0}, {101.0, 6.0}};
  snapshot1.asks = {{102.0, 7.0}, {103.0, 8.0}};
  book->applyBookUpdate(snapshot1);

  auto snapshot2 = updateFactory.create();
  snapshot2.type = BookUpdateType::SNAPSHOT;
  snapshot2.bids = {{199.0, 1.0}};
  snapshot2.asks = {{201.0, 1.0}};
  book->applyBookUpdate(snapshot2);

  for (double p = 190.0; p <= 210.0; ++p) {
    double bid = book->bidAtPrice(p);
    double ask = book->askAtPrice(p);
    if (bid > 0.0)
      EXPECT_GE(p, 190.0) << "Unexpected old bid at " << p;
    if (ask > 0.0)
      EXPECT_GE(p, 190.0) << "Unexpected old ask at " << p;
  }
}

TEST(OrderBookTest, MultipleExtremeDeltasAreHandledCorrectly) {
  constexpr double tickSize = 1.0;
  constexpr double deviation = 10.0; // 20 levels
  WindowedOrderBookFactory orderBookFactory;
  BookUpdateFactory bookUpdateFactory;
  auto book =
      orderBookFactory.create(WindowedOrderBookConfig{tickSize, deviation});

  // Initial snapshot
  auto snapshot = bookUpdateFactory.create();
  snapshot.type = BookUpdateType::SNAPSHOT;
  snapshot.bids = {{100.0, 5.0}};
  snapshot.asks = {{101.0, 5.0}};
  book->applyBookUpdate(snapshot);

  // First delta: add levels near best
  auto d1 = bookUpdateFactory.create();
  d1.type = BookUpdateType::DELTA;
  d1.bids = {{99.0, 2.0}, {98.0, 1.0}};
  d1.asks = {{102.0, 3.0}, {103.0, 4.0}};
  book->applyBookUpdate(d1);

  // Second delta: overwrite existing levels
  auto d2 = bookUpdateFactory.create();
  d2.type = BookUpdateType::DELTA;
  d2.bids = {{100.0, 6.0}, {98.0, 0.0}};  // remove 98
  d2.asks = {{101.0, 7.0}, {102.0, 0.0}}; // remove 102
  book->applyBookUpdate(d2);

  // Third delta: apply levels definitely outside the window
  auto d3 = bookUpdateFactory.create();
  d3.type = BookUpdateType::DELTA;
  d3.bids = {{80.0, 10.0}};  // well below basePrice
  d3.asks = {{120.0, 10.0}}; // well above window
  book->applyBookUpdate(d3);

  // Final state checks
  EXPECT_DOUBLE_EQ(book->bidAtPrice(100.0), 6.0); // updated
  EXPECT_DOUBLE_EQ(book->bidAtPrice(99.0), 2.0);  // added
  EXPECT_DOUBLE_EQ(book->bidAtPrice(98.0), 0.0);  // removed

  EXPECT_DOUBLE_EQ(book->askAtPrice(101.0), 7.0); // updated
  EXPECT_DOUBLE_EQ(book->askAtPrice(102.0), 0.0); // removed
  EXPECT_DOUBLE_EQ(book->askAtPrice(103.0), 4.0); // still present

  EXPECT_DOUBLE_EQ(book->bidAtPrice(80.0), 0.0);  // ignored
  EXPECT_DOUBLE_EQ(book->askAtPrice(120.0), 0.0); // ignored
}
