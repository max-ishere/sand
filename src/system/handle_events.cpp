#include <SDL2/SDL_events.h>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <entt/entity/fwd.hpp>
#include <sand/component/player_controllable.hpp>
#include <sand/systems.hpp>

constexpr void HandleKbEvents(entt::registry &, SDL_KeyboardEvent);

void HandleEvents(entt::registry &registry, bool &quit) {
  SDL_Event e;

  bool keydown = false;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      quit = true;
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      HandleKbEvents(registry, e.key);
      break;
    }
  }
}

constexpr void HandleKbEvents(entt::registry &registry, SDL_KeyboardEvent e) {
  constexpr float player_speed = 1;
  switch (e.keysym.sym) {
  case SDLK_w:
    if (e.state == SDL_PRESSED)
      registry.view<PlayerControllable>().each(
          [](PlayerControllable &controller) {
            controller.velocity.y = player_speed;
          });
    break;

  case SDLK_a:
    if (e.state == SDL_PRESSED)
      registry.view<PlayerControllable>().each(
          [](PlayerControllable &controller) {
            controller.velocity.y = -player_speed;
          });
    break;

  case SDLK_s:
    break;

  case SDLK_d:
    break;
  }
}

void HandlePlayerControl(entt::registry &registry) {
  registry.view<PlayerControllable, b2Body *>().each(
      [](PlayerControllable &controller, b2Body *physics_body) {
        const b2Vec2 &body_velocity = physics_body->GetLinearVelocity();

        if (controller.velocity.x < body_velocity.x)
          controller.velocity.x = body_velocity.x;

        if (controller.velocity.y < body_velocity.y)
          controller.velocity.y = body_velocity.y;

        physics_body->SetLinearVelocity(controller.velocity);
        controller.velocity = b2Vec2_zero;
      });
}
