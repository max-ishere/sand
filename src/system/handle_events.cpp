#include <SDL2/SDL_events.h>
#include <algorithm>
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <entt/entity/fwd.hpp>
#include <numbers>
#include <sand/component/player_controllable.hpp>
#include <sand/systems.hpp>

constexpr void HandleKbEvents(entt::registry &, SDL_KeyboardEvent);

void HandleEvents(entt::registry &registry, bool &quit, HUDstate &state) {
  SDL_Event e;

  bool keydown = false;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      quit = true;
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      HandleKbEvents(registry, e.key, state);
      break;
    }
  }
}

constexpr void HandleKbEvents(entt::registry &registry, SDL_KeyboardEvent e,
                              HUDstate &state) {
  constexpr float player_speed = 1;

  std::clamp(state.x, -1, 1);
  std::clamp(state.y, -1, 1);

  switch (e.keysym.sym) {
  case SDLK_w:
    if (e.state == SDL_PRESSED)
      registry.view<MovementIntent>().each([](auto &intent) {
        intent.velocity = 3;
        intent.angle = std::numbers::pi / 2;
      });
    else if (e.state == SDL_RELEASED)
      registry.view<MovementIntent>().each(
          [](auto &intent) { intent.velocity = 0; });
    break;

  case SDLK_a:
    if (e.state == SDL_PRESSED)
      registry.view<MovementIntent>().each([](auto &intent) {
        intent.velocity = 3;
        intent.angle = std::numbers::pi;
      });
    else if (e.state == SDL_RELEASED)
      registry.view<MovementIntent>().each(
          [](auto &intent) { intent.velocity = 0; });
    break;

  case SDLK_s:
    if (e.state == SDL_PRESSED)
      registry.view<MovementIntent>().each([](auto &intent) {
        intent.velocity = 3;
        intent.angle = std::numbers::pi * 3 / 2;
      });
    else if (e.state == SDL_RELEASED)
      registry.view<MovementIntent>().each(
          [](auto &intent) { intent.velocity = 0; });
    break;

  case SDLK_d:
    if (e.state == SDL_PRESSED)
      registry.view<MovementIntent>().each([](auto &intent) {
        intent.velocity = 3;
        intent.angle = 0;
      });
    else if (e.state == SDL_RELEASED)
      registry.view<MovementIntent>().each(
          [](auto &intent) { intent.velocity = 0; });
    break;
  }
}

void HandleControlIntents(entt::registry &registry) {
  registry.view<MovementIntent, b2Body *>().each(
      [](MovementIntent &intent, b2Body *physics_body) {
        const b2Vec2 &body_velocity = physics_body->GetLinearVelocity();

        b2Vec2 velocity(cos(intent.angle) * intent.velocity,
                        sin(intent.angle) * intent.velocity);

        physics_body->SetLinearVelocity(velocity);
      });
}
