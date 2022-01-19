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
  for (auto &line : map)
    for (auto &i : line)
      i = prng();
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

template <typename T, const size_t X, const size_t Y, const size_t new_x,
          const size_t new_y = new_x>
std::array<std::array<T, new_y>, new_x>
CropArray(std::array<std::array<T, Y>, X> src) {
  static_assert(new_x >= 2 and new_y >= 2,
                "This function should only be used for scaling up. Shrinking "
                "is not supported");
  static_assert(new_x < X and new_y < Y, "Cropping can not scale the array.");
  std::array<std::array<T, new_y>, new_x> result{};

  for (auto x = 0u; x < new_x; x++)
    for (auto y = 0u; y < new_y; y++)
      result[x][y] = src[x][y];

  return result;
}

template <typename T, const size_t X, const size_t Y,
          const bool wrap_for_tileability = false>
std::array<std::array<T, Y>, X>
BlurArray(std::array<std::array<T, Y>, X> src,
          const std::integral auto averaging_radius) {
  static_assert(
      X >= 2 and Y >= 2,
      "Arrays of size [1][...] and [0][...] are already \"blurred\".");

  std::array<std::array<T, Y>, X> blurred_array{T()};

  for (auto x = averaging_radius; x < X - averaging_radius; x++)
    for (auto y = averaging_radius; y < Y - averaging_radius; y++) {
      T sum{};
      for (auto x_offset = -averaging_radius; x_offset <= averaging_radius;
           x_offset++)
        for (auto y_offset = -averaging_radius; y_offset <= averaging_radius;
             y_offset++) {
          sum += src[x + x_offset][y + y_offset];
        }
      blurred_array[x][y] = sum / averaging_radius * averaging_radius * 2;
    }
  return blurred_array;
}
