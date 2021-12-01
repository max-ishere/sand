#pragma once
#include <entt/entity/fwd.hpp>
#include <sand/component/player_controllable.hpp>

void HandleEvents(entt::registry &, bool &quit, HUDstate &);
void HandleControlIntents(entt::registry &);
