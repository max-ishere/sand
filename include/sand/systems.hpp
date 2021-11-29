#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <box2d/box2d.h>
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <utility>

class Renderer {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  constexpr static auto position_to_pixels = 32;

  std::intmax_t last_get_ticks{}, current_get_ticks{};

public:
  struct CameraData {
    int x{}, y{}, hx_size{}, hy_size{};
  } camera_data;

  Renderer();
  ~Renderer();

  void operator()(entt::registry &);
  auto WindowSize() {
    int width = 0, height = 0;
    SDL_GetWindowSize(window, &width, &height);
    return std::pair<int, int>(width, height);
  }

  inline int GetDeltaTime() { return last_get_ticks - current_get_ticks; }
};

class Physics {
  static constexpr int32_t B2_RECOMENDED_VELOCITY_ITERATIONS = 8,
                           B2_RECOMENDED_POSITION_ITERATIONS = 3;

public:
  b2World world{b2Vec2_zero};
  void operator()(entt::registry &, float,
                  int32_t = B2_RECOMENDED_VELOCITY_ITERATIONS,
                  int32_t = B2_RECOMENDED_POSITION_ITERATIONS);
};

void ConvertPhysicsToRenderData(entt::registry &, Renderer::CameraData);
