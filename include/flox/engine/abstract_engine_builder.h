/*
 * Flox Engine
 * Developed by Evgenii Makarov (https://github.com/eeiaao)
 *
 * Copyright (c) 2025 Evgenii Makarov
 * Licensed under the MIT License. See LICENSE file in the project root for full
 * license information.
 */

#pragma once

#include "flox/engine/engine.h"
#include "flox/engine/engine_config.h"

#include <memory>

namespace flox {

class IEngineBuilder {
public:
  virtual ~IEngineBuilder() = default;

  virtual std::unique_ptr<Engine> build() = 0;
};

} // namespace flox
