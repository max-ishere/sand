#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <algorithm>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <iostream>
#include <sand/component/renderer_data.hpp>
#include <sand/system/renderer.hpp>
#include <sand/system/sprite_data.hpp>

constexpr void RenderGround(SDL_Renderer *, const SpriteData &, const int,
                            const int, const int, const int);

void Renderer::operator()(entt::registry &registry) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);

  static_assert(Renderer::position_to_pixels % 2 == 0,
                "Pixel scale should be divisible by 2");

  int width = 0, height = 0;
  SDL_GetWindowSize(window, &width, &height);

  int x_offset_camera = static_cast<int>(
      (camera_data.x - floor(camera_data.x)) * Renderer::position_to_pixels);
  int y_offset_camera =
      static_cast<int>((1.f - camera_data.y + floor(camera_data.y)) *
                       Renderer::position_to_pixels);

  RenderGround(renderer, sprite_data, width, height, x_offset_camera,
               y_offset_camera);

  int center_x = round((width - position_to_pixels) / 2),
      center_y = round((height - position_to_pixels) / 2);

  auto view = registry.view<RendererData, b2Body *>();
  view.each([this, center_x, center_y](const auto &renderer_data,
                                       const auto &body) {
    b2Vec2 position = body->GetPosition();
    SDL_Rect sprite_position{
        .x = static_cast<int>(
            round((position.x - camera_data.x + renderer_data.x_offset) *
                      position_to_pixels +
                  center_x)),
        .y = static_cast<int>(
            round(-(position.y - camera_data.y + renderer_data.y_offset) *
                      position_to_pixels +
                  center_y)),

        .w = position_to_pixels,
        .h = position_to_pixels,
    };
    if (!renderer_data.sprite) {
      SDL_RenderDrawRect(renderer, &sprite_position);
    } else {
      SDL_Rect sprite = sprite_data(renderer_data.sprite_id);
      SDL_RenderCopy(renderer, sprite_data.tilemap, &sprite, &sprite_position);
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
  sprite_data.tilemap = SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
}
Renderer::~Renderer() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
}

constexpr void RenderGround(SDL_Renderer *renderer,
                            const SpriteData &sprite_data, const int width,
                            const int height, const int x_offset_camera,
                            const int y_offset_camera) {
  const int grid_width = width / Renderer::position_to_pixels + 4,
            grid_height = height / Renderer::position_to_pixels + 4;

  int x_offset_edge =
      Renderer::position_to_pixels -
      static_cast<int>(round((width - Renderer::position_to_pixels) / 2)) %
          Renderer::position_to_pixels;
  int y_offset_edge =
      Renderer::position_to_pixels -
      static_cast<int>(round((height - Renderer::position_to_pixels) / 2)) %
          Renderer::position_to_pixels;

  for (auto i = 0u; i < grid_width; i++)
    for (auto j = 0u; j < grid_height; j++) {
      constexpr SDL_Rect grass{0, 16, 16, 16};

      SDL_Rect sprite_position{
          .x = static_cast<int>((i * Renderer::position_to_pixels) -
                                x_offset_edge - x_offset_camera),
          .y = static_cast<int>((j * Renderer::position_to_pixels) -
                                y_offset_edge - y_offset_camera),
          .w = Renderer::position_to_pixels,
          .h = Renderer::position_to_pixels,
      };

      SDL_RenderCopy(renderer, sprite_data.tilemap, &grass, &sprite_position);
    }
}
