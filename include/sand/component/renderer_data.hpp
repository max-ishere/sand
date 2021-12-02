#pragma once
#include <box2d/b2_math.h>
#include <sand/system/sprite_data.hpp>

struct RendererData {
  static constexpr auto z_normal_index = 64u;

  bool sprite{false};
  SpriteData::SpriteId sprite_id{};
  float x_offset{}, y_offset{};
  unsigned int z_index = z_normal_index;
};
