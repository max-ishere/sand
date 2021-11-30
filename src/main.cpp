#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <iostream>
#include <sand/component/player_controllable.hpp>
#include <sand/component/renderer_data.hpp>
#include <sand/core.hpp>
#include <sand/entity_factory.hpp>
#include <sand/systems.hpp>

int main(int argc, char *argv[]) {
  srand(time(NULL));

  const float fps = 30;

  Renderer Renderer;
  Physics Physics;
  Renderer.camera_data =
      Renderer::CameraData{.x = 0, .y = 0, .hx_size = 40, .hy_size = 40};

  entt::registry registry;

  for (auto i = 0u; i < 10u; i++) {
    const auto entity = registry.create();
    MakePhysicsEntity(registry, entity, Physics.world);

    const b2Vec2 position(rand() % 20 - 10, rand() % 20 - 10);
    auto body = registry.get<b2Body *>(entity);
    body->SetTransform(position, 0);

    b2PolygonShape shape;
    shape.SetAsBox(.5f, .5f);
    body->CreateFixture(&shape, 1)->SetFriction(5);
  }

  const auto player = registry.create();
  MakePhysicsEntity(registry, player, Physics.world);
  registry.emplace<MovementIntent>(player);
  b2PolygonShape shape;
  shape.SetAsBox(.5f, .5f);
  registry.get<b2Body *>(player)->CreateFixture(&shape, 1)->SetFriction(5);

  bool quit = false;
  while (!quit) {
    Timer timer;
    timer.start();

    HUDstate state;

    HandleEvents(registry, quit, state);
    HandleControlIntents(registry);
    Physics(registry, 1.f / fps);
    const auto position = registry.get<b2Body *>(player)->GetPosition();
    ConvertPhysicsToRenderData(
        registry, Renderer::CameraData{.x = position.x, .y = position.y});
    Renderer(registry);

    std::clog << "renderable entities: " << registry.view<RendererData>().size()
              << "\nphysics entities: " << registry.view<b2Body *>().size()
              << "\nwindow size: " << get<0>(Renderer.WindowSize()) << " "
              << "\n-----"
              << "Player data: pos: ("
              << registry.get<b2Body *>(player)->GetPosition().x << ", "
              << registry.get<b2Body *>(player)->GetPosition().y << ")"

              << get<1>(Renderer.WindowSize()) << "\n\n";

    timer.stop();
    if (timer.elapsedMilliseconds() < 1000.f / fps) {
      int delay = 1000 / fps - timer.elapsedMilliseconds();
      SDL_Delay((delay >= 0) ? delay : 0);
    }
  }
  return 0;
}
