#pragma once

#include "gfxAlgebra.hpp"
#include "gfxColor.hpp"

#include <iostream>

namespace gfx {

  struct triangle {
      point a, b, c;
      rgb24 ca, cb, cc;

      triangle(point a, point b, point c, rgb24 c_ab = WHITE, rgb24 c_bc = WHITE, rgb24 c_ca = WHITE)
          : a(a), b(b), c(c), ca(c_ab), cb(c_bc), cc(c_ca) {}

      triangle mul(const matrix& m) const {
        return {
            m.mul(a),
            m.mul(b),
            m.mul(c), ca, cb, cc};
      }
  };

  static std::ostream& operator<<(std::ostream& os, const triangle& t) {
    os << t.a << "|" << t.b << "|" << t.c;

    return os;
  }

  struct shader;
  struct screen_triangle {
      ipoint a, b, c;
      float da, db, dc;
      rgb24 ca = PURPLE, cb = GREEN, cc = YELLOW;

      screen_triangle(ipoint a, ipoint b, ipoint c, float da, float db, float dc, rgb24 c_ab = WHITE, rgb24 c_bc = WHITE, rgb24 c_ca = WHITE)
          : a(a), b(b), c(c), da(da), db(db), dc(dc), ca(c_ab), cb(c_bc), cc(c_ca) {}
  };

  static std::ostream& operator<<(std::ostream& os, const screen_triangle& t) {
    os << t.a << "|" << t.b << "|" << t.c;

    return os;
  }

} // namespace gfx