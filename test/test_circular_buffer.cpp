#include <exception>
#include <vector>
#include "gtest/gtest.h"


using namespace std;
using namespace ELB;


TEST(creation, invalid_size)
{
  EXPECT_THROW({
    CircularBuffer buffer(0);
  }, invalid_argument);
}

TEST(creation, valid_size)
{
  EXPECT_NO_THROW({
    CircularBuffer buffer(100);
  });
}

TEST(get_capacity, valid)
{
  constexpr uint32_t SIZE = 100;
  CircularBuffer buffer(SIZE);

  EXPECT_EQ(SIZE, buffer.GetCapacity());
}

TEST(get_free, valid)
{
  constexpr uint32_t SIZE = 100;
  CircularBuffer buffer(SIZE);

  EXPECT_EQ(0, buffer.GetFreeSize());
}

TEST(write, overflow)
{
  CircularBuffer buffer(10);
  vector<char> data(20, 'a');

  EXPECT_THROW({
    buffer.write(data, data.size());
  }, overflow_error);
}

TEST(write, valid)
{
  CircularBuffer buffer(10);
  vector<char> data(7, 'a');

  EXPECT_NO_THROW({
    buffer.write(data, data.size());
  }, overflow_error);
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}