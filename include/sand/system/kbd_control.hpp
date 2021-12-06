#pragma once
#include <entt/entity/fwd.hpp>
#include <sand/component/movement_intent.hpp>
#include <sand/component/player_controllable.hpp>

[[nodiscard]] MovementIntent HandleEvents(bool &quit, HUDstate &);
void HandleControlIntents(entt::registry &);
