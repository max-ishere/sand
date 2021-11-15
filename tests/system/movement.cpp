#include <gtest/gtest.h>
#include <sand/component/movement.hpp>
#include <sand/system/movement.hpp>
#include <sand/test/system/system_test.hpp>
namespace sand::test {
using namespace ::sand::component;

class MovementSystemTest : public SystemTest {
protected:
  const entt::entity tentity = tregistry.create();
  void SetUp() override { MakeMovable(tregistry, tentity); }
};

TEST_F(MovementSystemTest, MakeMovable) {
  auto movable_entity = tregistry.create();
  MakeMovable(tregistry, movable_entity);
  ASSERT_TRUE(
      (tregistry.all_of<Position, Velocity, Acceleration>(movable_entity)));
}

TEST_F(MovementSystemTest, Movement) {
  tregistry.emplace_or_replace<Acceleration>(tentity, 1.f, 0.f);
  tregistry.emplace_or_replace<Velocity>(tentity, 0.f, 0.f);
  tregistry.emplace_or_replace<Position>(tentity, 0.f, 0.f);

  float time_delta = 1.f;
  ::sand::system::Movement(tregistry, time_delta);

  ASSERT_EQ(tregistry.get<Acceleration>(tentity), (Acceleration{1.f, 0.f}))
      << "Should not change acceleration";
  ASSERT_EQ(tregistry.get<Velocity>(tentity), (Velocity{1.f, 0.f}))
      << "Should tick velocity from acceleration";
  ASSERT_EQ(tregistry.get<Position>(tentity), (Position{1.f, 0.f}))
      << "Should tick position in same call as velocity";
}
} // namespace sand::test
