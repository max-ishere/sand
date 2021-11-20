#include "sand/system/movement.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <sand/component/movement.hpp>
#include <sand/component/rendering.hpp>
#include <sand/sdl/system/rendering.hpp>
#include <sand/system/rendering.hpp>

int main(int argc, char *argv[]) {
  sand::sdl::system::SdlRendering Rendering;

  entt::registry registry;

  const auto entity = registry.create();
  sand::component::MakeMovable(registry, entity);
  const auto &velocity = registry.get<sand::component::Acceleration>(entity) =
      sand::component::Acceleration{.x = 2, .y = 1};

  registry.emplace<sand::component::gRectangle>(entity, 100, 50);

  Rendering(registry);

  SDL_bool quit = SDL_FALSE;
  int previous_time = 0, current_time = 0;
  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = SDL_TRUE;
        break;
      }
    }

    Rendering(registry);
    sand::system::Movement(registry, 1.f / 60.f);
    SDL_Delay(1000 / 60);
    if (quit == SDL_TRUE)
      break;
  }
  return 0;
}
