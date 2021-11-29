#pragma once
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <entt/entity/registry.hpp>

inline void MakePhysicsEntity(entt::registry &registry,
                              const entt::entity entity, b2World &world) {
  b2BodyDef body;
  body.type = b2_dynamicBody;
  registry.emplace<b2Body *>(entity, world.CreateBody(&body));
}
