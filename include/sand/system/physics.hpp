#pragma once
#include <box2d/box2d.h>
#include <entt/entity/fwd.hpp>

class Physics {
  static constexpr int32_t B2_RECOMENDED_VELOCITY_ITERATIONS = 8,
                           B2_RECOMENDED_POSITION_ITERATIONS = 3;

public:
  b2World world{b2Vec2_zero};
  void operator()(entt::registry &, float,
                  int32_t = B2_RECOMENDED_VELOCITY_ITERATIONS,
                  int32_t = B2_RECOMENDED_POSITION_ITERATIONS);
};
