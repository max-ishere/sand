#include <array>
#include <cstdint>
#include <cstdio>
#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-type-util.h>
#include <iostream>
#include <sand/core/noise.hpp>

template <typename T> class ScaleArrayTest : public ::testing::Test {
public:
  static const std::integral auto SIZE = 4u, SCALE = 2u;
  typedef std::array<std::array<T, SIZE>, SIZE> NormalSizeArray;
  typedef std::array<std::array<T, SIZE / SCALE>, SIZE / SCALE> SmallSizeArray;
  typedef std::array<std::array<T, SIZE * SCALE>, SIZE * SCALE> BigSizeArray;
};

using IntegralTypes = ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t>;

TYPED_TEST_SUITE(ScaleArrayTest, IntegralTypes);

TYPED_TEST(ScaleArrayTest, ScaleUp) {
  /// Should fill area SCALE * SCALE in result with values from src
  typename TestFixture::NormalSizeArray src = {0};
  {
    auto val = 0;
    for (auto &line : src)
      for (auto &i : line)
        i = val++;
  }

  typename TestFixture::BigSizeArray result =
      ScaleArray<TypeParam, TestFixture::SIZE, TestFixture::SIZE,
                 TestFixture::SCALE>(src);

#ifdef SHOW_ARRAY_FOR_THIS_TEST

  for (auto line : src) {
    for (auto i : line)
      std::cout << (int)i << ' ';
    std::cout << '\n';
  }
  std::cout << '\n';
  for (auto line : result) {
    for (auto i : line)
      std::cout << (int)i << ' ';
    std::cout << '\n';
  }

#endif

  for (auto x = 0u; x < TestFixture::SIZE; x++)
    for (auto y = 0u; y < TestFixture::SIZE; y++)

      for (auto x_offset = 0u; x_offset < TestFixture::SCALE; x_offset++)
        for (auto y_offset = 0u; y_offset < TestFixture::SCALE; y_offset++)

          EXPECT_EQ(src[x][y], result[x * TestFixture::SCALE + x_offset]
                                     [y * TestFixture::SCALE + y_offset])
              << "x: " << x << "\ny: " << y << "\nx offset: " << x_offset
              << "\ny offset: " << y_offset;
}
