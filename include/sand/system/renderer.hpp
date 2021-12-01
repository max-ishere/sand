#pragma once
#include <SDL2/SDL.h>
#include <entt/entity/fwd.hpp>

class Renderer {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  constexpr static auto position_to_pixels = 64;

  SDL_Texture *tilemap = nullptr;

public:
  struct CameraData {
    float x{}, y{}, hx_size{}, hy_size{};
    template <typename T> void operator=(T t) {
      x = t.x;
      y = t.y;
    }
  };
  CameraData camera_data;

  Renderer();
  ~Renderer();

  void operator()(entt::registry &);
  auto WindowSize() {
    int width = 0, height = 0;
    SDL_GetWindowSize(window, &width, &height);
    return std::pair<int, int>(width, height);
  }
};

struct RenderEnable {};
