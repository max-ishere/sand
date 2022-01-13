#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <sand/component/living_thing.hpp>
#include <sand/system/life.hpp>
#include <type_traits>

void Hunger(entt::registry &registry) {
  registry.view<LivingThing>().each([](LivingThing &living_thing) {
    if (not living_thing.alive)
      return;

    static_assert(std::is_unsigned_v<decltype(living_thing.ticks_until_hungry)>,
                  "if statement assumes unsigned type");
    if (living_thing.ticks_until_hungry != 0) {
      living_thing.ticks_until_hungry--;
    }
  });
}
