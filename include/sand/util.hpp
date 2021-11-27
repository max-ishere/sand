#pragma once

#include <entt/entity/registry.hpp>
namespace sand::util {
template <typename... C>
constexpr bool has_components(const entt::registry &registry,
                              const entt::entity entity) {
  return registry.all_of<C...>(entity);
}
} // namespace sand::util
