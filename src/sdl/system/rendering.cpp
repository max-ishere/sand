#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <entt/entity/registry.hpp>
#include <sand/component/movement.hpp>
#include <sand/component/rendering.hpp>
#include <sand/sdl/system/rendering.hpp>
#include <string>

sand::sdl::system::SdlRendering::SdlRendering(
    const uint32_t init_flags, const SdlWindowProperties &win_proerties,
    const SdlRendererProperties &renderer_properties) {
  SDL_Init(init_flags);
  if ((window = SDL_CreateWindow(
           win_proerties.title.c_str(), win_proerties.x, win_proerties.y,
           win_proerties.w, win_proerties.h, win_proerties.flags)) == nullptr) {
    throw std::runtime_error(
        std::string("Could not create SDL window. SDL_GetError: ") +
        SDL_GetError() + "\n");
  }
  if ((window_surface = SDL_GetWindowSurface(window)) == nullptr) {
    throw std::runtime_error(
        std::string(
            "Could not create SDL renderer for main windown. SDL_GetError: ") +
        SDL_GetError() + "\n");
  }
  if ((renderer = SDL_CreateRenderer(window, renderer_properties.driver_index,
                                     renderer_properties.flags)) == nullptr) {
    throw std::runtime_error(
        std::string("Could not create renderer with specified properties. "
                    "SDL_GetError: ") +

        SDL_GetError() + "\n");
  }
  SDL_SetWindowResizable(window, SDL_TRUE);
  SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xFF, 0);
}

sand::sdl::system::SdlRendering::~SdlRendering() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}

void sand::sdl::system::SdlRendering::operator()(entt::registry &registry) {
  SDL_SetRenderDrawColor(renderer, 0xAA, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
  registry.view<component::gRectangle, component::Position>().each(
      [this](const auto &rectangle, const auto &position) {
        SDL_Rect sdl_rect{(int)(position.x * 10), (int)(position.y * 10),
                          rectangle.w, rectangle.h};
        SDL_RenderFillRect(renderer, &sdl_rect);
      });

  SDL_RenderPresent(renderer);
}
