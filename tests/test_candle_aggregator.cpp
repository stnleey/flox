/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/aggregator/candle_aggregator.h"
#include "flox/book/trade.h"

#include <gtest/gtest.h>

using namespace flox;

namespace {

constexpr SymbolId SYMBOL = 42;
const std::chrono::seconds INTERVAL = std::chrono::seconds(60);

std::chrono::system_clock::time_point ts(int seconds) {
  return std::chrono::system_clock::time_point(std::chrono::seconds(seconds));
}

} // namespace

// Verifies basic candle aggregation: open, high, low, close, volume, and flush
// on interval change
TEST(CandleAggregatorTest, AggregatesTradesIntoCandles) {
  std::vector<Candle> result;
  CandleAggregator aggregator(
      INTERVAL, [&](SymbolId, const Candle &c) { result.push_back(c); });

  aggregator.start();

  aggregator.onTrade(
      Trade{.symbol = SYMBOL, .price = 100, .quantity = 1, .timestamp = ts(0)});
  aggregator.onTrade(Trade{
      .symbol = SYMBOL, .price = 105, .quantity = 2, .timestamp = ts(10)});
  aggregator.onTrade(
      Trade{.symbol = SYMBOL, .price = 99, .quantity = 3, .timestamp = ts(20)});
  aggregator.onTrade(Trade{
      .symbol = SYMBOL, .price = 101, .quantity = 1, .timestamp = ts(30)});
  aggregator.onTrade(Trade{.symbol = SYMBOL,
                           .price = 102,
                           .quantity = 2,
                           .timestamp = ts(65)}); // triggers flush

  ASSERT_EQ(result.size(), 1);
  EXPECT_DOUBLE_EQ(result[0].open, 100);
  EXPECT_DOUBLE_EQ(result[0].high, 105);
  EXPECT_DOUBLE_EQ(result[0].low, 99);
  EXPECT_DOUBLE_EQ(result[0].close, 101);
  EXPECT_DOUBLE_EQ(result[0].volume, 7);
  EXPECT_EQ(result[0].startTime, ts(0));
  EXPECT_EQ(result[0].endTime, ts(60));
}

// Ensures the final candle is emitted on stop() if it's still open
TEST(CandleAggregatorTest, FlushesFinalCandleOnStop) {
  std::vector<Candle> result;
  CandleAggregator aggregator(
      INTERVAL, [&](SymbolId, const Candle &c) { result.push_back(c); });

  aggregator.start();

  aggregator.onTrade(
      Trade{.symbol = SYMBOL, .price = 100, .quantity = 1, .timestamp = ts(0)});
  aggregator.onTrade(Trade{
      .symbol = SYMBOL, .price = 105, .quantity = 1, .timestamp = ts(30)});

  aggregator.stop(); // flush remaining

  ASSERT_EQ(result.size(), 1);
  EXPECT_DOUBLE_EQ(result[0].open, 100);
  EXPECT_DOUBLE_EQ(result[0].high, 105);
  EXPECT_DOUBLE_EQ(result[0].low, 100);
  EXPECT_DOUBLE_EQ(result[0].close, 105);
  EXPECT_DOUBLE_EQ(result[0].volume, 2);
}

// Ensures a new candle is started after a gap (timestamp exceeds current
// interval)
TEST(CandleAggregatorTest, StartsNewCandleAfterGap) {
  std::vector<Candle> result;
  CandleAggregator aggregator(
      INTERVAL, [&](SymbolId, const Candle &c) { result.push_back(c); });

  aggregator.start();

  aggregator.onTrade(
      Trade{.symbol = SYMBOL, .price = 110, .quantity = 1, .timestamp = ts(0)});
  aggregator.onTrade(Trade{.symbol = SYMBOL,
                           .price = 120,
                           .quantity = 2,
                           .timestamp = ts(130)}); // gap → flush

  ASSERT_EQ(result.size(), 1);
  EXPECT_EQ(result[0].startTime, ts(0));
  EXPECT_EQ(result[0].endTime, ts(60));
  EXPECT_DOUBLE_EQ(result[0].close, 110);
}

// Tests a candle that consists of a single trade (OHLC must all equal that
// price)
TEST(CandleAggregatorTest, SingleTradeCandle) {
  std::vector<Candle> result;
  CandleAggregator aggregator(
      INTERVAL, [&](SymbolId, const Candle &c) { result.push_back(c); });

  aggregator.start();
  aggregator.onTrade(
      Trade{.symbol = SYMBOL, .price = 123, .quantity = 1, .timestamp = ts(5)});
  aggregator.stop(); // flush

  ASSERT_EQ(result.size(), 1);
  const auto &candle = result[0];
  EXPECT_DOUBLE_EQ(candle.open, 123);
  EXPECT_DOUBLE_EQ(candle.high, 123);
  EXPECT_DOUBLE_EQ(candle.low, 123);
  EXPECT_DOUBLE_EQ(candle.close, 123);
  EXPECT_DOUBLE_EQ(candle.volume, 1);
}

// Checks that trades from different symbols are aggregated into separate
// candles
TEST(CandleAggregatorTest, MultipleSymbolsAreAggregatedSeparately) {
  std::vector<std::pair<SymbolId, Candle>> result;
  CandleAggregator aggregator(INTERVAL, [&](SymbolId symbol, const Candle &c) {
    result.emplace_back(symbol, c);
  });

  aggregator.start();

  aggregator.onTrade(
      Trade{.symbol = 1, .price = 10, .quantity = 1, .timestamp = ts(0)});
  aggregator.onTrade(
      Trade{.symbol = 2, .price = 20, .quantity = 2, .timestamp = ts(10)});
  aggregator.onTrade(
      Trade{.symbol = 1, .price = 12, .quantity = 1, .timestamp = ts(30)});
  aggregator.onTrade(
      Trade{.symbol = 2, .price = 18, .quantity = 1, .timestamp = ts(40)});

  aggregator.stop(); // flush all

  ASSERT_EQ(result.size(), 2);

  auto it1 = std::find_if(result.begin(), result.end(),
                          [](const auto &p) { return p.first == 1; });
  auto it2 = std::find_if(result.begin(), result.end(),
                          [](const auto &p) { return p.first == 2; });

  ASSERT_TRUE(it1 != result.end());
  ASSERT_TRUE(it2 != result.end());

  EXPECT_DOUBLE_EQ(it1->second.volume, 2);
  EXPECT_DOUBLE_EQ(it2->second.volume, 3);
}

// Verifies that calling start() clears any previous internal state and candles
TEST(CandleAggregatorTest, DoubleStartClearsOldState) {
  std::vector<Candle> result;
  CandleAggregator aggregator(
      INTERVAL, [&](SymbolId, const Candle &c) { result.push_back(c); });

  aggregator.start();
  aggregator.onTrade(
      Trade{.symbol = SYMBOL, .price = 100, .quantity = 1, .timestamp = ts(0)});
  aggregator.start(); // clears previous state
  aggregator.onTrade(Trade{
      .symbol = SYMBOL, .price = 105, .quantity = 2, .timestamp = ts(65)});
  aggregator.stop();

  ASSERT_EQ(result.size(), 1);
  EXPECT_DOUBLE_EQ(result[0].open, 105);
  EXPECT_DOUBLE_EQ(result[0].volume, 2);
  EXPECT_EQ(result[0].startTime, ts(60));
}
