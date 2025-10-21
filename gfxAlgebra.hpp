#pragma once

#include <ostream>

namespace gfx {
  struct point {
      float x = 0.0f, y = 0.0f, z = 0.0f;
  };

  static std::ostream& operator<<(std::ostream& os, const point& t) {
    os << "(" << t.x << "," << t.y << "," << t.z << ")";

    return os;
  }

  struct ipoint {
      int x = 0, y = 0;
  };

  static std::ostream& operator<<(std::ostream& os, const ipoint& t) {
    os << "(" << t.x << "," << t.y << ")";

    return os;
  }

  struct matrix {
      double a11, a12, a13, a14;
      double a21, a22, a23, a24;
      double a31, a32, a33, a34;
      double a41, a42, a43, a44;

      static matrix identity() {
        return {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
      }

      static matrix ortho(float width, float height, float z_far, float z_near) {
        return {
            1.0f / width, .0f, .0f, .0f,
            .0f, 1.0f / height, .0f, .0f,
            .0f, .0f, -2.0f / (z_far - z_near), -(z_far + z_near) / (z_far - z_near),
            .0f, .0f, .0f, 1.0f};
      }

      static matrix view(const point& eye, const point& target, const point& up) {
        // Forward (camera direction)
        double fx = target.x - eye.x;
        double fy = target.y - eye.y;
        double fz = target.z - eye.z;
        double flen = std::sqrt(fx * fx + fy * fy + fz * fz);
        fx /= flen;
        fy /= flen;
        fz /= flen;

        // Right = normalize(cross(forward, up))
        double rx = fy * up.z - fz * up.y;
        double ry = fz * up.x - fx * up.z;
        double rz = fx * up.y - fy * up.x;
        double rlen = std::sqrt(rx * rx + ry * ry + rz * rz);
        rx /= rlen;
        ry /= rlen;
        rz /= rlen;

        // Recompute true up = cross(right, forward)
        double ux = ry * fz - rz * fy;
        double uy = rz * fx - rx * fz;
        double uz = rx * fy - ry * fx;

        // Translation
        double tx = -(rx * eye.x + ry * eye.y + rz * eye.z);
        double ty = -(ux * eye.x + uy * eye.y + uz * eye.z);
        double tz = -(fx * eye.x + fy * eye.y + fz * eye.z);

        return {
            rx, ux, -fx, 0.0,
            ry, uy, -fy, 0.0,
            rz, uz, -fz, 0.0,
            tx, ty, -tz, 1.0};
      }

      point mul(const point& p) const {
        double x = p.x * a11 + p.y * a12 + p.z * a13 + a14;
        double y = p.x * a21 + p.y * a22 + p.z * a23 + a24;
        double z = p.x * a31 + p.y * a32 + p.z * a33 + a34;
        double w = p.x * a41 + p.y * a42 + p.z * a43 + a44;

        if (w != 0.0)
          return {static_cast<float>(x / w),
                  static_cast<float>(y / w),
                  static_cast<float>(z / w)};
        else
          return {static_cast<float>(x),
                  static_cast<float>(y),
                  static_cast<float>(z)};
      }

      static matrix rotation_x(double angle_rad) {
        double c = std::cos(angle_rad);
        double s = std::sin(angle_rad);
        return {
            1, 0, 0, 0,
            0, c, -s, 0,
            0, s, c, 0,
            0, 0, 0, 1};
      }

      static matrix rotation_y(double angle_rad) {
        double c = std::cos(angle_rad);
        double s = std::sin(angle_rad);
        return {
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1};
      }

      static matrix rotation_z(double angle_rad) {
        double c = std::cos(angle_rad);
        double s = std::sin(angle_rad);
        return {
            c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
      }

      matrix mul(const matrix& m) const {
        matrix r{};
        r.a11 = a11 * m.a11 + a12 * m.a21 + a13 * m.a31 + a14 * m.a41;
        r.a12 = a11 * m.a12 + a12 * m.a22 + a13 * m.a32 + a14 * m.a42;
        r.a13 = a11 * m.a13 + a12 * m.a23 + a13 * m.a33 + a14 * m.a43;
        r.a14 = a11 * m.a14 + a12 * m.a24 + a13 * m.a34 + a14 * m.a44;

        r.a21 = a21 * m.a11 + a22 * m.a21 + a23 * m.a31 + a24 * m.a41;
        r.a22 = a21 * m.a12 + a22 * m.a22 + a23 * m.a32 + a24 * m.a42;
        r.a23 = a21 * m.a13 + a22 * m.a23 + a23 * m.a33 + a24 * m.a43;
        r.a24 = a21 * m.a14 + a22 * m.a24 + a23 * m.a34 + a24 * m.a44;

        r.a31 = a31 * m.a11 + a32 * m.a21 + a33 * m.a31 + a34 * m.a41;
        r.a32 = a31 * m.a12 + a32 * m.a22 + a33 * m.a32 + a34 * m.a42;
        r.a33 = a31 * m.a13 + a32 * m.a23 + a33 * m.a33 + a34 * m.a43;
        r.a34 = a31 * m.a14 + a32 * m.a24 + a33 * m.a34 + a34 * m.a44;

        r.a41 = a41 * m.a11 + a42 * m.a21 + a43 * m.a31 + a44 * m.a41;
        r.a42 = a41 * m.a12 + a42 * m.a22 + a43 * m.a32 + a44 * m.a42;
        r.a43 = a41 * m.a13 + a42 * m.a23 + a43 * m.a33 + a44 * m.a43;
        r.a44 = a41 * m.a14 + a42 * m.a24 + a43 * m.a34 + a44 * m.a44;
        return r;
      }

      static matrix rotation(double x_rad, double y_rad, double z_rad) {
        return rotation_z(z_rad).mul(rotation_y(y_rad)).mul(rotation_x(x_rad));
      }
  };
} // namespace gfx