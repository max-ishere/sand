#include <sand/component/movement.hpp>
#include <sand/system/movement.hpp>

namespace sand::system {
void Movement(entt::registry &registry, float delta) {
  registry.view<component::Acceleration, component::Velocity>().each(
      [delta](const auto &acceleration, auto &velocity) {
        velocity += acceleration * delta;
      });
  registry.view<component::Velocity, component::Position>().each(
      [delta](const auto &velocity, auto &position) {
        position += velocity * delta;
      });
}
} // namespace sand::system
