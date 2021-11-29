#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <sand/component/renderer_data.hpp>
#include <sand/systems.hpp>

void Renderer::operator()(entt::registry &registry) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);

  int width = 0, height = 0;
  SDL_GetWindowSize(window, &width, &height);

  static_assert(Renderer::position_to_pixels % 2 == 0,
                "Pixel scale should be divisible by 2");

  registry.view<RendererData>().each(
      [this, width, height](const auto &renderer_data) {
        SDL_Rect rect{
            .x = (int)(round(renderer_data.x * position_to_pixels)) -
                 camera_data.x * position_to_pixels -
                 (int)(position_to_pixels / 2) + (int)round(width / 2),
            .y = (int)(round(renderer_data.y * position_to_pixels)) -
                 camera_data.y * position_to_pixels -
                 (int)(position_to_pixels / 2) + (int)round(height / 2),
            .w = position_to_pixels,
            .h = position_to_pixels,
        };
        SDL_RenderDrawRect(renderer, &rect);
      });

  SDL_RenderPresent(renderer);
  last_get_ticks = current_get_ticks;
  current_get_ticks = SDL_GetTicks();
}

namespace {
void SDLassert(void *ptr) {
#ifdef DEBUG
  if (ptr == nullptr) {
    std::cerr << "SDL error: " << SDL_GetError() << "\n";
  }
#endif
}
} // namespace

Renderer::Renderer() {
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("Sand Game", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 640, 480,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  SDLassert(window);

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDLassert(renderer);
}
Renderer::~Renderer() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
