/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#include "flox/aggregator/candle_aggregator.h"

namespace flox {

CandleAggregator::CandleAggregator(std::chrono::seconds interval,
                                   CandleCallback callback)
    : _interval(interval), _callback(callback) {}

std::chrono::system_clock::time_point
CandleAggregator::alignToInterval(std::chrono::system_clock::time_point tp) {
  auto t =
      std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch())
          .count();
  t = (t / _interval.count()) * _interval.count();
  return std::chrono::system_clock::time_point(std::chrono::seconds(t));
}

void CandleAggregator::start() { _candles.clear(); }
void CandleAggregator::stop() {
  for (auto &[symbol, partial] : _candles) {
    if (partial.initialized) {
      _callback(symbol, partial.candle);
    }
  }
  _candles.clear();
}

void CandleAggregator::onTrade(const Trade &trade) {
  auto aligned = alignToInterval(trade.timestamp);
  auto &partial = _candles[trade.symbol];

  if (!partial.initialized || partial.candle.endTime <= trade.timestamp) {
    if (partial.initialized) {
      _callback(trade.symbol, partial.candle);
    }

    partial = PartialCandle{.candle = Candle{.open = trade.price,
                                             .high = trade.price,
                                             .low = trade.price,
                                             .close = trade.price,
                                             .volume = trade.quantity,
                                             .startTime = aligned,
                                             .endTime = aligned + _interval},
                            .initialized = true};
  } else {
    partial.candle.high = std::max(partial.candle.high, trade.price);
    partial.candle.low = std::min(partial.candle.low, trade.price);
    partial.candle.close = trade.price;
    partial.candle.volume += trade.quantity;
  }
}

} // namespace flox
