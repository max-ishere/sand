#pragma once
#include <array>
#include <cstdint>

struct BiomeGridBlock {
  uint8_t humidity{}, temperature{};
};

template <std::integral auto x, std::integral auto y>
void GenerateBiomes(std::array<std::array<BiomeGridBlock, x>, y>) {
  static_assert(x != 0 and y != 0);
}
