#include "gtest/gtest.h"


TEST(circular_buffer, invalid_size)
{
  FAIL();
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}