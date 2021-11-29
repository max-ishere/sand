#include <box2d/b2_body.h>
#include <box2d/b2_time_step.h>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <sand/component/renderer_data.hpp>
#include <sand/systems.hpp>
void Physics::operator()(entt::registry &registry, float b2TimeStep,
                         int32_t velocityIterations,
                         int32_t positionIterations) {
  world.Step(b2TimeStep, velocityIterations, positionIterations);
}

void ConvertPhysicsToRenderData(entt::registry &registry,
                                Renderer::CameraData camera_data) {
  registry.view<b2Body *>().each([&registry, camera_data](const auto entity,
                                                          const b2Body *body) {
    b2Vec2 position = body->GetPosition();

    position.x -= camera_data.x;
    position.y -= camera_data.y;
    registry.emplace_or_replace<RendererData>(entity, position.x, position.y);
  });
}
