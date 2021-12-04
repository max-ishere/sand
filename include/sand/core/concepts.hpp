#pragma once
#include <type_traits>

template <typename C> constexpr auto is_coordinate = requires(C c) {
  c.x;
  c.y;

  std::is_arithmetic<decltype(C::x)>();
  std::is_arithmetic<decltype(C::y)>();
};

template <typename C>
concept Coordinate = is_coordinate<C>;
