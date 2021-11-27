#include "sand/box2d/system/physics.hpp"
#include "sand/util.hpp"
#include <box2d/b2_body.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <entt/entity/fwd.hpp>
#include <gtest/gtest.h>
#include <sand/component/position.hpp>
#include <sand/system/physics.hpp>
#include <sand/test/system/system_test.hpp>

namespace sand::test {
class PhysicsSystemTest : public SystemTest {

protected:
  b2World tworld{b2Vec2_zero};
  const entt::entity tentity{tregistry.create()};

  void SetUp() { box2d::system::MakePhysicsEntity(tregistry, tworld, tentity); }
};

TEST_F(PhysicsSystemTest, MakePhysicsEntity) {
  const auto local_tentity = tregistry.create();

  component::Transform tsand_transform{.x = 1, .y = 2, .angle = .5f};
  b2BodyDef tbodydef;

  box2d::system::MakePhysicsEntity(tregistry, tworld, local_tentity, tbodydef);

  ASSERT_TRUE((util::has_components<component::Transform, b2Body *>(
      tregistry, local_tentity)));

  auto sand_transform = tregistry.get<component::Transform>(local_tentity);
  auto b2_transform = tregistry.get<b2Body *>(local_tentity)->GetTransform();
  EXPECT_EQ(sand_transform.x, b2_transform.p.x);
  EXPECT_EQ(sand_transform.y, b2_transform.p.y);
  EXPECT_EQ(sand_transform.angle, b2_transform.q.GetAngle());
}

TEST_F(PhysicsSystemTest, box2d_implementation) {
  tregistry.get<b2Body *>(tentity)->SetLinearVelocity(b2Vec2{1, 1});
  box2d::system::Physics(tregistry, tworld);
  tregistry.view<component::Transform, b2Body *>().each(
      [](const auto &position, const auto &body) {
        EXPECT_EQ(body->GetPosition().x, position.x);
        EXPECT_EQ(body->GetPosition().y, position.y);
      });
}
} // namespace sand::test
