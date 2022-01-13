#include "sand/system/ai.hpp"
#include <sand/core/main_loop.hpp>
#include <sand/system/life.hpp>

void tick(entt::registry &registry) {
  ProcessAI(registry);
  Hunger(registry);
}
