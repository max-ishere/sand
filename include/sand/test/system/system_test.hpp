#include <entt/entt.hpp>
#include <gtest/gtest.h>

namespace sand::test {
class SystemTest : public ::testing::Test {
protected:
  entt::registry tregistry{};
  const entt::entity tentity = tregistry.create();
};
} // namespace sand::test
