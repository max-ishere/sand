#include "sand/system/movement.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <cstdlib>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <sand/component/movement.hpp>
#include <sand/component/rendering.hpp>
#include <sand/sdl/system/event.hpp>
#include <sand/sdl/system/rendering.hpp>
#include <sand/system/event.hpp>
#include <sand/system/rendering.hpp>

int main(int argc, char *argv[]) {
  sand::sdl::system::SdlRendering SdlRendering;
  sand::system::Rendering &Rendering = SdlRendering;

  sand::sdl::system::SdlEventPoller SdlEventPoller;
  sand::system::EventPoller &EventPoller = SdlEventPoller;

  entt::registry registry;

  const auto entity = registry.create();
  sand::component::MakeMovable(registry, entity);
  const auto &velocity = registry.get<sand::component::Acceleration>(entity) =
      sand::component::Acceleration{.x = 2, .y = 1};

  registry.emplace<sand::component::gRectangle>(entity, 100, 50);

  while (true) {
    EventPoller();
    if (EventPoller.should_quit())
      break;

    sand::system::Movement(registry, 1.f / 60.f);
    Rendering(registry);
    SDL_Delay(1000 / 60);
  }
  return 0;
}
