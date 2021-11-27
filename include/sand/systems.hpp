#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <entt/entity/registry.hpp>

class Renderer {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  constexpr static auto position_to_pixels = 16;

public:
  SDL_Point camera_position{0, 0};
  Renderer();
  ~Renderer();

  void operator()(entt::registry &);
};
