#pragma once

namespace gfx {
  struct rgb24 {
      unsigned char r, g, b;
  };

  constexpr rgb24 BLACK{0, 0, 0};
  constexpr rgb24 WHITE{255, 255, 255};
  constexpr rgb24 RED{255, 0, 0};
  constexpr rgb24 GREEN{0, 255, 0};
  constexpr rgb24 BLUE{0, 0, 255};
  constexpr rgb24 CYAN{0, 255, 255};
  constexpr rgb24 PURPLE{255, 0, 255};
  constexpr rgb24 YELLOW{255, 255, 0};
} // namespace gfx