#pragma once
#include <SDL2/SDL_events.h>
#include <queue>
#include <sand/system/event.hpp>

namespace sand::sdl::system {

class SdlEventPoller : public ::sand::system::EventPoller {
  std::queue<SDL_Event> events;
  bool quit = false;

public:
  void operator()() final override;
  bool should_quit() final override;
};

} // namespace sand::sdl::system
