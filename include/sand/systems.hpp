#pragma once
#include <box2d/box2d.h>
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <sand/component/player_controllable.hpp>
#include <sand/system/renderer.hpp>
#include <utility>

class Physics {
  static constexpr int32_t B2_RECOMENDED_VELOCITY_ITERATIONS = 8,
                           B2_RECOMENDED_POSITION_ITERATIONS = 3;

public:
  b2World world{b2Vec2_zero};
  void operator()(entt::registry &, float,
                  int32_t = B2_RECOMENDED_VELOCITY_ITERATIONS,
                  int32_t = B2_RECOMENDED_POSITION_ITERATIONS);
};

void ConvertPhysicsToRenderData(entt::registry &, Renderer::CameraData);

void HandleEvents(entt::registry &, bool &quit, HUDstate &);

void HandleControlIntents(entt::registry &);
