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
  constexpr uint32_t SIZE = 10;
  CircularBuffer buffer(SIZE);
  EXPECT_EQ(SIZE, buffer.GetCapacity());

  vector<char> data(20, 'a');

  EXPECT_THROW({
    buffer.Write(data, data.size());
  }, overflow_error);

  EXPECT_EQ(SIZE, buffer.GetFreeSize());
}

TEST(write, valid)
{
  constexpr uint32_t SIZE = 10;
  CircularBuffer buffer(SIZE);

  vector<char> data(7, 'a');

  EXPECT_NO_THROW({
    buffer.Write(data, data.size());
  });

  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());
}


TEST(read, overflow)
{
  constexpr uint32_t SIZE = 100;
  CircularBuffer buffer(SIZE);

  vector<char> data(20, 'a');
  buffer.Write(data, data.size());
  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());

  EXPECT_THROW({
    vector<char> data = buffer.Read(87);
  }, out_of_range);

  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());
}

TEST(read, valid)
{
  CircularBuffer buffer(10);
  vector<char> data(17, 'a');

  EXPECT_NO_THROW({
    buffer.write(data, data.size());
  });
  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());

  EXPECT_NO_THROW({
    vector<char> data = buffer.Read(8);
  });

  EXPECT_EQ(SIZE - data.size() + 8, buffer.GetFreeSize());
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}