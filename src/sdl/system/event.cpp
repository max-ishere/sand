#include <SDL2/SDL_events.h>
#include <sand/sdl/system/event.hpp>

void sand::sdl::system::SdlEventPoller::operator()() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      quit = true;
      break;
    default:
      events.push(event);
    }
  }
}

bool sand::sdl::system::SdlEventPoller::should_quit() { return quit; }
