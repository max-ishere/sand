#pragma once
#include <SDL2/SDL.h>

class Renderer {
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  constexpr static auto position_to_pixels = 32;

public:
  struct CameraData {
    float x{}, y{}, hx_size{}, hy_size{};
  } camera_data;

  Renderer();
  ~Renderer();

  void operator()(entt::registry &);
  auto WindowSize() {
    int width = 0, height = 0;
    SDL_GetWindowSize(window, &width, &height);
    return std::pair<int, int>(width, height);
  }
};
