#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>
#include <entt/entity/registry.hpp>
#include <sand/component/renderer_data.hpp>
#include <sand/systems.hpp>

int main(int argc, char *argv[]) {
  Renderer Renderer;

  Renderer.camera_position = SDL_Point{.x = -10, .y = -10};

  entt::registry registry;

  for (auto i = 0u; i < 10u; i++) {
    registry.emplace<RendererData>(registry.create(), i, i + 1);
  }

  SDL_Event e;
  while (e.type != SDL_QUIT) {
    Renderer(registry);
    SDL_PollEvent(&e);
  }
  return 0;
}
