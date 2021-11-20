#pragma once
#include <cstdint>
#include <entt/entity/fwd.hpp>
#include <string>
namespace sand::system {

class Rendering {
public:
  const static std::string game_title;
  virtual void operator()(entt::registry &) = 0;
};
} // namespace sand::system
