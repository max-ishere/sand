#include <entt/entity/registry.hpp>
#include <numbers>
#include <sand/component/ai.hpp>
#include <sand/component/movement_intent.hpp>
#include <sand/system/ai.hpp>

void ProcessAI(entt::registry &registry) {
  registry.view<AI>().each([&registry](const entt::entity agent) {
    registry.emplace_or_replace<MovementIntent>(
        agent, MovementIntent{.angle = static_cast<float>(rand() % 8 *
                                                          std::numbers::pi / 4),
                              .velocity = 1});
  });
}
