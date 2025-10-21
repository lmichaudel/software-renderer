#pragma once

#include "gfxBuffer.hpp"
#include "gfxColor.hpp"
#include "gfxShader.hpp"
#include "gfxTriangle.hpp"

#include <cstddef>

namespace gfx {
  struct renderer {
      size_t width, height;
      buffer<rgb24> color_buffer;
      buffer<float> depth_buffer;

      renderer(size_t width, size_t height)
          : width(width), height(height), color_buffer(width, height), depth_buffer(width, height) {}

      void clear() {
        color_buffer.clear(BLACK);
        depth_buffer.clear(1.0f);
      }

      void draw(triangle t, shader& s, const matrix& view, const matrix& proj, const matrix& model) {
        // Apply vertex shader
        triangle transformed{s.vertex(t.a, view, proj, model),
                             s.vertex(t.b, view, proj, model),
                             s.vertex(t.c, view, proj, model), t.ca, t.cb, t.cc};

        screen_triangle st{
            {static_cast<int>((transformed.a.x + 1.0f) / 2.0f * width), static_cast<int>((transformed.a.y + 1.0f) / 2.0f * height)},
            {static_cast<int>((transformed.b.x + 1.0f) / 2.0f * width), static_cast<int>((transformed.b.y + 1.0f) / 2.0f * height)},
            {static_cast<int>((transformed.c.x + 1.0f) / 2.0f * width), static_cast<int>((transformed.c.y + 1.0f) / 2.0f * height)},
            transformed.a.z,
            transformed.b.z,
            transformed.c.z,
            transformed.ca,
            transformed.cb,
            transformed.cc};

        // step 2 : rasterize
        int minX = std::min({st.a.x, st.b.x, st.c.x});
        int maxX = std::max({st.a.x, st.b.x, st.c.x});
        int minY = std::min({st.a.y, st.b.y, st.c.y});
        int maxY = std::max({st.a.y, st.b.y, st.c.y});

        // Precompute area (for barycentric)
        float denom = float((st.b.y - st.c.y) * (st.a.x - st.c.x) + (st.c.x - st.b.x) * (st.a.y - st.c.y));
        if (denom == 0.0f)
          return; // Degenerate triangle

        for (int y = minY; y <= maxY; ++y) {
          for (int x = minX; x <= maxX; ++x) {
            // Compute barycentric coordinates
            float w1 = float((st.b.y - st.c.y) * (x - st.c.x) + (st.c.x - st.b.x) * (y - st.c.y)) / denom;
            float w2 = float((st.c.y - st.a.y) * (x - st.c.x) + (st.a.x - st.c.x) * (y - st.c.y)) / denom;
            float w3 = 1.0f - w1 - w2;

            // Inside test
            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
              float d = w1 * st.da + w2 * st.db + w3 * st.dc;
              if (!(x >= color_buffer.width || x < 0 || y >= color_buffer.height || y < 0) && d <= depth_buffer(x, y)) {
                depth_buffer(x, y) = d;
                color_buffer(x, y) = s.fragment(st, w1, w2, w3);
              }
            }
          }
        }
      }
  };
} // namespace gfx