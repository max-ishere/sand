#include <box2d/b2_math.h>
#include <gtest/gtest.h>
#include <sand/core/z_index.hpp>

namespace {
const std::integral auto DEFAULT = 1, LESS = DEFAULT - 1, GREATER = DEFAULT + 1;

const b2Vec2 ABOVE(DEFAULT, GREATER), BELOW(DEFAULT, LESS), LEFT(LESS, DEFAULT),
    RIGHT(GREATER, DEFAULT), CENTER(DEFAULT, DEFAULT);
} // namespace

TEST(Zindex, VerticalOrder) {
  EXPECT_TRUE(TopDownOrdered(ABOVE, CENTER));
  EXPECT_FALSE(TopDownOrdered(BELOW, CENTER));
  EXPECT_FALSE(TopDownOrdered(CENTER, CENTER));
}

TEST(Zindex, HorizontalOrder) {
  EXPECT_TRUE(LeftRightOrdered(LEFT, CENTER));
  EXPECT_FALSE(LeftRightOrdered(RIGHT, CENTER));
  EXPECT_FALSE(LeftRightOrdered(CENTER, CENTER));
}

TEST(Zindex, ZindexLess) {
  EXPECT_FALSE(ZindexLessOrdered(CENTER, DEFAULT, CENTER, DEFAULT));
  EXPECT_TRUE(ZindexLessOrdered(CENTER, LESS, CENTER, DEFAULT));
  EXPECT_FALSE(ZindexLessOrdered(CENTER, GREATER, CENTER, DEFAULT));

  EXPECT_TRUE(ZindexLessOrdered(ABOVE, DEFAULT, CENTER, DEFAULT));
  EXPECT_FALSE(ZindexLessOrdered(BELOW, DEFAULT, CENTER, DEFAULT));
  EXPECT_TRUE(ZindexLessOrdered(LEFT, DEFAULT, CENTER, DEFAULT));
  EXPECT_FALSE(ZindexLessOrdered(RIGHT, DEFAULT, CENTER, DEFAULT));

  EXPECT_FALSE(ZindexLessOrdered(ABOVE, GREATER, CENTER, DEFAULT));
  EXPECT_TRUE(ZindexLessOrdered(ABOVE, LESS, CENTER, DEFAULT));

  EXPECT_TRUE(ZindexLessOrdered(BELOW, LESS, CENTER, DEFAULT));
  EXPECT_FALSE(ZindexLessOrdered(BELOW, GREATER, CENTER, DEFAULT));

  EXPECT_FALSE(ZindexLessOrdered(LEFT, GREATER, CENTER, DEFAULT));
  EXPECT_TRUE(ZindexLessOrdered(LEFT, LESS, CENTER, DEFAULT));

  EXPECT_TRUE(ZindexLessOrdered(RIGHT, LESS, CENTER, GREATER));
  EXPECT_FALSE(ZindexLessOrdered(RIGHT, GREATER, CENTER, GREATER));
}
