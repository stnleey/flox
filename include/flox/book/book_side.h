/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory_resource>
#include <optional>
#include <vector>

namespace flox {

class BookSide {
public:
  enum class Side { Bid, Ask };

  BookSide(std::size_t windowSize, Side side, std::pmr::memory_resource *mem);

  void setLevel(std::size_t index, double qty);
  double getLevel(std::size_t index) const;

  void shift(int levels);
  void clear();

  std::optional<std::size_t> findBest() const;

  std::pmr::memory_resource *allocator() const {
    return _qty.get_allocator().resource();
  }

protected:
  std::size_t ring(std::size_t index) const {
    return (index + _offset) % _windowSize;
  }

  std::pmr::vector<double> _qty;
  std::size_t _offset = 0;
  std::size_t _windowSize;
  Side _side;

  std::optional<std::size_t> _bestIndex;
};

} // namespace flox
