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
#include <functional>
#include <iostream>
#include <sand/component/player_controllable.hpp>
#include <sand/component/renderer_data.hpp>
#include <sand/core.hpp>
#include <sand/core/z_index.hpp>
#include <sand/entity_factory.hpp>
#include <sand/systems.hpp>

int main(int argc, char *argv[]) {
  srand(time(NULL));

  const float fps = 24;

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
    registry.emplace<RendererData>(
        entity, RendererData{.sprite = true,
                             .sprite_id = SpriteData::SpriteId::Box,
                             .z_index = RendererData::z_normal_index});
  }

  const auto box = registry.create();
  MakePhysicsEntity(registry, box, Physics.world);

  const b2Vec2 position(0, -1);
  auto body = registry.get<b2Body *>(box);
  body->SetTransform(position, 0);
  body->SetType(b2_staticBody);

  b2PolygonShape box_shape;
  box_shape.SetAsBox(.5f, .5f);

  body->CreateFixture(&box_shape, 1)->SetFriction(5);
  registry.emplace<RendererData>(box);

  const auto player = registry.create();
  MakePhysicsEntity(registry, player, Physics.world);
  registry.emplace<MovementIntent>(player);

  b2CircleShape shape;
  shape.m_radius = 0x1p-3;
  registry.get<b2Body *>(player)->CreateFixture(&shape, 1)->SetFriction(5);
  registry.emplace<RendererData>(player, true, SpriteData::SpriteId::Character,
                                 0.f, 0.5 - shape.m_radius,
                                 RendererData::z_normal_index + 1);

  auto frame_conter = 0u;

  bool quit = false;
  while (!quit) {
    frame_conter++;
    Timer timer;
    timer.start();

    HUDstate state;

    HandleEvents(registry, quit, state);
    HandleControlIntents(registry);
    Physics(registry, 1.f / fps);

    const b2Vec2 &position = registry.get<b2Body *>(player)->GetPosition();
    Renderer.camera_data = position;

    const RendererData &render_data = registry.get<RendererData>(player);
    Renderer.camera_data.x += render_data.x_offset;
    Renderer.camera_data.y += render_data.y_offset;

    if (frame_conter % (int)fps == 0)
      registry.sort<RendererData>([&registry](
                                      const entt::entity lentity,
                                      const entt::entity rentity) -> bool {
        const auto &lposition = registry.get<b2Body *>(lentity)->GetPosition(),
                   rposition = registry.get<b2Body *>(rentity)->GetPosition();

        const auto lindex = registry.get<RendererData>(lentity).z_index,
                   rindex = registry.get<RendererData>(rentity).z_index;

        return ZindexLessOrdered(lposition, lindex, rposition, rindex);
      });

    Renderer(registry);

    std::clog << "renderable entities: " << registry.view<RendererData>().size()
              << "\nphysics entities: " << registry.view<b2Body *>().size()
              << "\nwindow size: " << get<0>(Renderer.WindowSize()) << " "
              << get<1>(Renderer.WindowSize()) << "\n\n"
              << "\n-----"
              << "Player data: pos: ("
              << registry.get<b2Body *>(player)->GetPosition().x << ", "
              << registry.get<b2Body *>(player)->GetPosition().y << ")";

    timer.stop();
    if (timer.elapsedMilliseconds() < 1000.f / fps) {
      int delay = 1000 / fps - timer.elapsedMilliseconds();
      SDL_Delay((delay >= 0) ? delay : 0);
    }
  }
  return 0;
}
