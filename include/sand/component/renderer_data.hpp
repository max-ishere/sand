#pragma once
#include <box2d/b2_math.h>
#include <sand/system/sprite_data.hpp>
struct RendererData {
  bool sprite{false};
  SpriteData::SpriteId sprite_id{};
  float x_offset{}, y_offset{};
};
