#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <exception>
#include <sand/system/rendering.hpp>
#include <string>
namespace sand::sdl::system {

typedef struct SdlWindowProperties {
  std::string title = ::sand::system::Rendering::game_title;
  int x = SDL_WINDOWPOS_CENTERED, y = SDL_WINDOWPOS_CENTERED, w = 640, h = 480;
  std::uint32_t flags = SDL_WINDOW_OPENGL;
} SdlWindowProperties;

typedef struct SdlRendererProperties {
  int driver_index = -1;
  uint32_t flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
} SdlRendererProperties;

class SdlRendering : public ::sand::system::Rendering {
  SDL_Window *window = nullptr;
  SDL_Surface *window_surface = nullptr;
  SDL_Renderer *renderer = nullptr;

public:
  SdlRendering(const uint32_t init_flags = SDL_INIT_VIDEO,
               const SdlWindowProperties & = SdlWindowProperties{},
               const SdlRendererProperties & = SdlRendererProperties{});
  ~SdlRendering();
  void operator()(entt::registry &) final override;
};
} // namespace sand::sdl::system
