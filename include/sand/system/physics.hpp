#pragma once
#include "sand/component/position.hpp"
#include <entt/entity/registry.hpp>
#include <sand/util.hpp>
namespace sand::system {

enum class PhysicsBodyType {
  Dynamic = 0,
  Static,
};
}
