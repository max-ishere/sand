#pragma once
#include <box2d/b2_math.h>
#include <concepts>
#include <sand/core/concepts.hpp>

template <Coordinate Position>
bool TopDownOrdered(Position origin, Position relative) {
  return (origin.y > relative.y);
}

template <Coordinate Position>
bool LeftRightOrdered(Position origin, Position relative) {
  return (origin.x < relative.x);
}

template <Coordinate C>
bool ZindexLessOrdered(const C &lposition, const std::integral auto lindex,
                       const C &rposition, const std::integral auto rindex) {
  return (lindex == rindex) ? ((TopDownOrdered(lposition, rposition))
                                   ? true
                                   : LeftRightOrdered(lposition, rposition))
                            : (lindex < rindex);
}
