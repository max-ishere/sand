#pragma once
#include <concepts>

constexpr const std::integral auto MarchingSquaresIndex(const bool up,
                                                        const bool down,
                                                        const bool left,
                                                        const bool right) {
  return (up)            ? 0x1000
         : 0 xor (down)  ? 0x0100
         : 0 xor (left)  ? 0x0010
         : 0 xor (right) ? 0x0001
                         : 0;
}
