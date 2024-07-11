#include <exception>
#include <vector>
#include "gtest/gtest.h"
#include "circular_buffer/circular_buffer.hpp"


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

  EXPECT_EQ(SIZE, buffer.GetFreeSize());
}

TEST(write, overflow)
{
  constexpr uint32_t SIZE = 10;
  CircularBuffer buffer(SIZE);
  EXPECT_EQ(SIZE, buffer.GetCapacity());

  vector<char> data(20, 'a');

  EXPECT_THROW({
    buffer.Write(data);
  }, overflow_error);

  EXPECT_EQ(SIZE, buffer.GetFreeSize());
}

TEST(write, valid)
{
  constexpr uint32_t SIZE = 10;
  CircularBuffer buffer(SIZE);

  vector<char> data(7, 'a');

  EXPECT_NO_THROW({
    buffer.Write(data);
  });

  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());
}


TEST(read, overflow)
{
  constexpr uint32_t SIZE = 100;
  CircularBuffer buffer(SIZE);

  vector<char> data(20, 'a');
  buffer.Write(data);
  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());

  EXPECT_THROW({
    vector<char> data = buffer.Read(87);
  }, out_of_range);

  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());
}

TEST(read, valid)
{
  constexpr uint32_t SIZE = 100;
  CircularBuffer buffer(SIZE);

  constexpr uint32_t WRITE_SIZE = 17;
  constexpr uint32_t READ_SIZE = 8;
  vector<char> data(WRITE_SIZE, 'a');

  EXPECT_NO_THROW({
    buffer.Write(data);
  });
  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());

  EXPECT_NO_THROW({
    data = buffer.Read(READ_SIZE);
  });

  EXPECT_EQ(SIZE - WRITE_SIZE + READ_SIZE, buffer.GetFreeSize());
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}