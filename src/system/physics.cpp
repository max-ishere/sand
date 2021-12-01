#include <box2d/b2_body.h>
#include <box2d/b2_time_step.h>
#include <entt/entity/registry.hpp>
#include <sand/component/renderer_data.hpp>
#include <sand/system/physics.hpp>
#include <sand/system/renderer.hpp>
void Physics::operator()(entt::registry &registry, float b2TimeStep,
                         int32_t velocityIterations,
                         int32_t positionIterations) {
  world.Step(b2TimeStep, velocityIterations, positionIterations);
}
