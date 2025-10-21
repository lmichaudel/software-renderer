#pragma once

#include "gfxColor.hpp"

#include <ostream>
#include <vector>

namespace gfx {

  template <typename T>
  struct buffer {
      size_t width, height;
      std::vector<T> surface;

      buffer<T>(size_t width, size_t height) : width(width), height(height), surface(width * height) {}

      void clear(T color) {
        for (auto it = surface.begin(); it != surface.end(); it++) {
          *it = color;
        }
      }

      const T& operator()(size_t x, size_t y) const {
        return surface[x + y * width];
      }

      T& operator()(size_t x, size_t y) {
        return surface[x + y * width];
      }
  };

  static std::ostream& operator<<(std::ostream& os, const buffer<rgb24>& p) {
    os << "P6" << std::endl
       << p.width << " " << p.height << std::endl
       << "255" << std::endl;
    os.write(reinterpret_cast<const char*>(p.surface.data()), p.surface.size() * sizeof(rgb24));

    return os;
  }

  static std::ostream& operator<<(std::ostream& os, const buffer<float>& p) {
    os << "P5" << std::endl
       << p.width << " " << p.height << std::endl
       << "255" << std::endl;

    for (auto it = p.surface.begin(); it != p.surface.end(); it++) {
      os << static_cast<unsigned char>(255 * *(it));
    }

    return os;
  }
} // namespace gfx