#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstdint>
#include <map>

class SpriteData {
public:
  SDL_Texture *tilemap{nullptr};
  static const auto pixels_per_sprite = 16u;
  enum class SpriteId : uintmax_t {
    Character = 0u,
    Grass,
    Box,
  };

#define SPRITE_SIZE pixels_per_sprite, pixels_per_sprite
#define SPRITE_GRID(x, y) pixels_per_sprite *x, pixels_per_sprite *y

  [[nodiscard]] constexpr SDL_Rect operator()(SpriteId id) const {
    switch (id) {
    case SpriteId::Character:
      return SDL_Rect{SPRITE_GRID(1, 0), SPRITE_SIZE};
      break;
    case SpriteId::Grass:
      return SDL_Rect{SPRITE_GRID(0, 1), SPRITE_SIZE};
      break;
    case SpriteId::Box:
      return SDL_Rect{SPRITE_GRID(0, 2), SPRITE_SIZE};
      break;
    default:
      return SDL_Rect{};
    }
  }
#undef SPRITE_SIZE
#undef SPRITE_GRID
};
