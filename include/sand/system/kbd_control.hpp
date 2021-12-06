#pragma once
#include <entt/entity/fwd.hpp>
#include <sand/component/movement_intent.hpp>
#include <sand/component/player_controllable.hpp>

[[nodiscard]] MovementIntent HandleEvents(entt::registry &, bool &quit,
                                          HUDstate &);
void HandleControlIntents(entt::registry &);
