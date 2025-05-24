/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "book_update.h"
#include <array>
#include <memory_resource>

namespace flox {

class BookUpdateFactory {
public:
  BookUpdateFactory()
      : _arena(_buffer.data(), _buffer.size()), _pool(&_arena) {}

  BookUpdate create() { return BookUpdate(&_pool); }

private:
  std::array<std::byte, 32 * 1024> _buffer;
  std::pmr::monotonic_buffer_resource _arena;
  std::pmr::unsynchronized_pool_resource _pool;
};

} // namespace flox
