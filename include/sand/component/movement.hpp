#pragma once

#include <concepts>
#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
#include <type_traits>

#ifdef DEBUG
#include <iostream>
#endif

namespace sand::component {

template <typename C> constexpr auto is_coordinate = requires(C c) {
  c.x;
  c.y;

  std::is_arithmetic<decltype(C::x)>();
  std::is_arithmetic<decltype(C::y)>();
};

template <typename C>
concept Coordinate = is_coordinate<C>;

template <Coordinate lCoordinate, Coordinate rCoordinate>
bool operator==(const lCoordinate &lhs, const rCoordinate &rhs) {
  return lhs.x == rhs.x and lhs.y == rhs.y;
}

template <Coordinate Coordinate, typename Arithmetic,
          std::enable_if_t<std::is_arithmetic<Arithmetic>::value, bool> = true>
const Coordinate operator*(const Coordinate &coordinate, Arithmetic delta) {
  return Coordinate{.x = coordinate.x * delta, .y = coordinate.y * delta};
}

template <Coordinate lCoordinate, Coordinate rCoordinate>
const lCoordinate operator*=(const lCoordinate &lhs, const rCoordinate &rhs) {
  return lCoordinate{.x = lhs.x * rhs.x, .y = lhs.y * rhs.y};
}

template <Coordinate lCoordinate, Coordinate rCoordinate>
constexpr lCoordinate &operator+=(lCoordinate &lhs, const rCoordinate &rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;

  return lhs;
}

#ifdef DEBUG
template <Coordinate C> std::ostream &operator<<(std::ostream &os, const C &c) {
  os << "Coordinate(.x = " << c.x << ", .y = " << c.y << ");";
  return os;
}
#endif

typedef struct Position {
  float x{}, y{};
} Position;

typedef struct Acceleration {
  float x{}, y{};
} Acceleration;

typedef struct Velocity {
  float x{}, y{};
} Velocity;

inline static void MakeMovable(entt::registry &registry, entt::entity entity) {
  registry.emplace<Acceleration>(entity);
  registry.emplace<Velocity>(entity);
  registry.emplace<Position>(entity);
}
} // namespace sand::component
