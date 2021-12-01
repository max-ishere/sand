#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <entt/entity/registry.hpp>
#include <iostream>
#include <sand/component/renderer_data.hpp>
#include <sand/system/renderer.hpp>
void Renderer::operator()(entt::registry &registry) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);

  int width = 0, height = 0;
  SDL_GetWindowSize(window, &width, &height);

  int grid_width = width / position_to_pixels + 4,
      grid_height = height / position_to_pixels + 4;

  for (auto i = 0u; i < grid_width; i++)
    for (auto j = 0u; j < grid_height; j++) {
      constexpr SDL_Rect grass{0, 16, 16, 16};

      // position of left edge of the sprite(on the edge) relative to left edge
      // of the screen
      int x_offset_edge =
          position_to_pixels -
          static_cast<int>(round((width - position_to_pixels) / 2)) %
              position_to_pixels;
      int y_offset_edge =
          position_to_pixels -
          static_cast<int>(round((height - position_to_pixels) / 2)) %
              position_to_pixels;

      // position of the character in local coordinates relative to the closes
      // tile

      int x_offset_camera = static_cast<int>(
          (camera_data.x - floor(camera_data.x)) * position_to_pixels);
      int y_offset_camera = static_cast<int>(
          (1.f - camera_data.y + floor(camera_data.y)) * position_to_pixels);

      SDL_Rect rect{
          .x = static_cast<int>((i * position_to_pixels) - x_offset_edge -
                                x_offset_camera),
          .y = static_cast<int>((j * position_to_pixels) - y_offset_edge -
                                y_offset_camera),
          .w = position_to_pixels,
          .h = position_to_pixels,

      };

      SDL_RenderCopy(renderer, tilemap, &grass, &rect);
    }

  static_assert(Renderer::position_to_pixels % 2 == 0,
                "Pixel scale should be divisible by 2");

  registry.view<RendererData, b2Body *>().each(
      [this, width, height](const auto &renderer_data, const auto &body) {
        b2Vec2 position = body->GetPosition();
        SDL_Rect rect{
            .x = static_cast<int>(
                (round((position.x - camera_data.x) * position_to_pixels)) -
                (int)(position_to_pixels / 2) + (int)round(width / 2)),
            .y = static_cast<int>(
                -(round((position.y - camera_data.y) * position_to_pixels)) -
                (int)(position_to_pixels / 2) + (int)round(height / 2)),
            .w = position_to_pixels,
            .h = position_to_pixels,
        };
        if (!renderer_data.sprite) {
          SDL_RenderDrawRect(renderer, &rect);
        } else {
          SDL_Rect sprite_size{.x = 0, .y = 0, .w = 16, .h = 16};
          SDL_RenderCopy(renderer, tilemap, &sprite_size, &rect);
        }
      });

  SDL_RenderPresent(renderer);
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

  SDL_Surface *temp_surface = SDL_LoadBMP("assets/tilemap.bmp");
  SDLassert(temp_surface);
  tilemap = SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
}
Renderer::~Renderer() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}
