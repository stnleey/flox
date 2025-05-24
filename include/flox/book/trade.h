/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include <chrono>
#include <string>

#include "flox/common.h"

namespace flox {

struct Trade {
  SymbolId symbol;
  double price;
  double quantity;
  bool isBuy;
  std::chrono::system_clock::time_point timestamp;
};

} // namespace flox
