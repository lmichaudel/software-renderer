#include "gfx.hpp"

#include "SDL2/SDL.h"

#define RAD(x) ((x) * 3.1415f / 180.0f)
#define IPOINT(x, y) ((gfx::ipoint){x, y})
#define POINT(x, y, z) ((gfx::point){x, y, z})

gfx::renderer gfx_renderer{800, 600};

std::vector<gfx::triangle>
    cube = {
        gfx::triangle(POINT(-1.0f, -1.0f, 1.0f), POINT(1.0f, -1.0f, 1.0f), POINT(1.0f, 1.0f, 1.0f), gfx::PURPLE, gfx::BLUE, gfx::GREEN),
        gfx::triangle(POINT(1.0f, 1.0f, 1.0f), POINT(-1.0f, 1.0f, 1.0f), POINT(-1.0f, -1.0f, 1.0f), gfx::GREEN, gfx::BLUE, gfx::PURPLE),
        gfx::triangle(POINT(-1.0f, -1.0f, -1.0f), POINT(-1.0f, 1.0f, -1.0f), POINT(1.0f, 1.0f, -1.0f), gfx::GREEN, gfx::YELLOW, gfx::RED),
        gfx::triangle(POINT(1.0f, 1.0f, -1.0f), POINT(1.0f, -1.0f, -1.0f), POINT(-1.0f, -1.0f, -1.0f), gfx::RED, gfx::YELLOW, gfx::GREEN),
        gfx::triangle(POINT(-1.0f, -1.0f, -1.0f), POINT(-1.0f, -1.0f, 1.0f), POINT(-1.0f, 1.0f, 1.0f), gfx::GREEN, gfx::YELLOW, gfx::RED),
        gfx::triangle(POINT(-1.0f, 1.0f, 1.0f), POINT(-1.0f, 1.0f, -1.0f), POINT(-1.0f, -1.0f, -1.0f), gfx::RED, gfx::YELLOW, gfx::GREEN),
        gfx::triangle(POINT(1.0f, -1.0f, 1.0f), POINT(1.0f, -1.0f, -1.0f), POINT(1.0f, 1.0f, -1.0f), gfx::PURPLE, gfx::PURPLE, gfx::PURPLE),
        gfx::triangle(POINT(1.0f, 1.0f, -1.0f), POINT(1.0f, 1.0f, 1.0f), POINT(1.0f, -1.0f, 1.0f), gfx::PURPLE, gfx::PURPLE, gfx::PURPLE),
        gfx::triangle(POINT(-1.0f, 1.0f, 1.0f), POINT(1.0f, 1.0f, 1.0f), POINT(1.0f, 1.0f, -1.0f), gfx::CYAN, gfx::RED, gfx::PURPLE),
        gfx::triangle(POINT(1.0f, 1.0f, -1.0f), POINT(-1.0f, 1.0f, -1.0f), POINT(-1.0f, 1.0f, 1.0f), gfx::PURPLE, gfx::RED, gfx::CYAN),
        gfx::triangle(POINT(-1.0f, -1.0f, -1.0f), POINT(1.0f, -1.0f, -1.0f), POINT(1.0f, -1.0f, 1.0f), gfx::YELLOW, gfx::GREEN, gfx::BLUE),
        gfx::triangle(POINT(1.0f, -1.0f, 1.0f), POINT(-1.0f, -1.0f, 1.0f), POINT(-1.0f, -1.0f, -1.0f), gfx::BLUE, gfx::GREEN, gfx::YELLOW)};

gfx::matrix view = gfx::matrix::identity();
gfx::matrix proj = gfx::matrix::ortho((800.0f / 600.0f) * 2.0f, 2.0f, 100.0f, -100.0f);
gfx::matrix model = gfx::matrix::identity();

gfx::point vertex(gfx::point input, const gfx::matrix& view, const gfx::matrix& proj, const gfx::matrix& model) {
  input = model.mul(input);
  input = proj.mul(input);
  input = view.mul(input);

  return input;
}

gfx::rgb24 fragment(gfx::screen_triangle triangle, float w1, float w2, float w3) {
  gfx::rgb24 col;
  col.r = uint8_t(w1 * triangle.ca.r + w2 * triangle.cb.r + w3 * triangle.cc.r);
  col.g = uint8_t(w1 * triangle.ca.g + w2 * triangle.cb.g + w3 * triangle.cc.g);
  col.b = uint8_t(w1 * triangle.ca.b + w2 * triangle.cb.b + w3 * triangle.cc.b);

  // Simple barycentric coloring
  return col;
}

gfx::shader shader{std::function(vertex), std::function(fragment)};

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Software Renderer", 300, 300, 800, 600, SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Texture* texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           800, 600);

  bool running = true;
  SDL_Event event;
  float theta = 0.0f;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = false;
    }

    model = gfx::matrix::rotation(RAD(theta), RAD(theta), RAD(theta));
    theta += 45.0f * 0.016f;

    gfx_renderer.clear();

    for (gfx::triangle t : cube) {
      gfx_renderer.draw(t, shader, view, proj, model);
    }

    std::vector<uint32_t> temp(800 * 600);
    for (int i = 0; i < 800 * 600; ++i) {
      unsigned char r = gfx_renderer.color_buffer.surface[i].r;
      unsigned char g = gfx_renderer.color_buffer.surface[i].g;
      unsigned char b = gfx_renderer.color_buffer.surface[i].b;
      temp[i] = (255 << 24) | (r << 16) | (g << 8) | b;
    }

    SDL_UpdateTexture(texture, nullptr, temp.data(), 800 * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}