#pragma once
#include "sand/component/position.hpp"
#include "sand/system/physics.hpp"
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/box2d.h>
#include <entt/entity/registry.hpp>

namespace sand::box2d::system {
void Physics(entt::registry &, b2World &world);
static void MakePhysicsEntity(entt::registry &registry, b2World &world,
                              const entt::entity &entity,
                              b2BodyDef bodydef = b2BodyDef{}) {
  registry.emplace<b2Body *>(entity, world.CreateBody(&bodydef));
}
} // namespace sand::box2d::system
