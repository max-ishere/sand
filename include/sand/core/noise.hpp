#pragma once
#include "sand/core/random.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <functional>
#include <sand/core/random.hpp>
#include <type_traits>

template <typename T, const size_t X, const size_t Y>
std::array<std::array<T, Y>, X> RandomNoiseMap(FastRandom prng) {
  static_assert(X != 0 and Y != 0);
  std::array<std::array<T, Y>, X> map = {};
  for (auto x = 0u; x < X; x++)
    for (auto y = 0u; y < Y; y++)
      map[x][y] = prng();
  return map;
}

template <typename T, const size_t X, const size_t Y, const size_t scale_x,
          const size_t scale_y = scale_x>
std::array<std::array<T, Y * scale_y>, X * scale_x>
ScaleArray(std::array<std::array<T, Y>, X> src) {
  static_assert(scale_x >= 2 and scale_y >= 2,
                "This function should only be used for scaling up. Shrinking "
                "is not supported");
  std::array<std::array<T, Y * scale_y>, X * scale_x> result{};

  for (auto x = 0u; x < X; x++)
    for (auto x_offset = 0u; x_offset < scale_x; x_offset++)

      for (auto y = 0u; y < Y; y++)
        for (auto y_offset = 0u; y_offset < scale_y; y_offset++)

          result[x * scale_x + x_offset][y * scale_y + y_offset] = src[x][y];

  return result;
}
