#pragma once

#include "gfxTriangle.hpp"

#include <functional>

namespace gfx {
  struct shader {
      std::function<point(point, const matrix&, const matrix&, const matrix&)> vertex;
      std::function<rgb24(screen_triangle, float, float, float)> fragment;
  };
} // namespace gfx