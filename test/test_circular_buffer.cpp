#include <vector>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "circular_buffer/circular_buffer.hpp"


using namespace std;
using namespace std::this_thread;
using namespace std::chrono;
using namespace ELB;


TEST(creation, invalid_size)
{
  EXPECT_THROW({
    CircularBuffer buffer{0};
  }, invalid_argument);
}

TEST(creation, valid_size)
{
  EXPECT_NO_THROW({
    CircularBuffer buffer{100};
  });
}

TEST(get_capacity, valid)
{
  constexpr uint32_t SIZE{100};
  CircularBuffer buffer{SIZE};

  EXPECT_EQ(SIZE, buffer.GetCapacity());
}

TEST(get_free, valid)
{
  constexpr uint32_t SIZE{100};
  CircularBuffer buffer{SIZE};

  EXPECT_EQ(SIZE, buffer.GetFreeSize());
}

TEST(write, overflow)
{
  constexpr uint32_t SIZE{10};
  CircularBuffer buffer{SIZE};
  EXPECT_EQ(SIZE, buffer.GetCapacity());

  vector<char> data(20, 'a');

  EXPECT_THROW({
    buffer.Write(data);
  }, overflow_error);

  EXPECT_EQ(SIZE, buffer.GetFreeSize());
}

TEST(write, valid)
{
  constexpr uint32_t SIZE{10};
  CircularBuffer buffer{SIZE};

  vector<char> data(7, 'a');

  EXPECT_NO_THROW({
    buffer.Write(data);
  });

  EXPECT_EQ(SIZE - data.size(), buffer.GetFreeSize());
}

TEST(write, iterative)
{
  std::srand(std::time(nullptr));

  constexpr uint32_t CAPACITY{1 * 1024 * 1024};
  CircularBuffer buffer{CAPACITY};

  EXPECT_EQ(CAPACITY, buffer.GetFreeSize());
  EXPECT_EQ(CAPACITY, buffer.GetCapacity());
  EXPECT_EQ(0, buffer.GetSize());

  uint32_t total_size{CAPACITY};

  while (total_size) {
    EXPECT_EQ(total_size, buffer.GetFreeSize());

    const uint32_t random_size{static_cast<uint32_t>(256 * (std::rand() / (float)RAND_MAX))};
    const uint32_t write_size{std::min(random_size, buffer.GetFreeSize())};
    const vector<char> data(write_size, 'a');

    buffer.Write(data);
    total_size -= write_size;

    EXPECT_EQ(total_size, buffer.GetFreeSize());
  }

  EXPECT_EQ(0, buffer.GetFreeSize());
  EXPECT_EQ(CAPACITY, buffer.GetCapacity());
  EXPECT_EQ(CAPACITY, buffer.GetSize());
}

TEST(read, overflow)
{
  constexpr uint32_t SIZE{100};
  CircularBuffer buffer{SIZE};

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
  constexpr uint32_t SIZE{100};
  CircularBuffer buffer{SIZE};

  constexpr uint32_t WRITE_SIZE{17};
  constexpr uint32_t READ_SIZE{8};
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

TEST(read, iterative)
{
  std::srand(std::time(nullptr));

  constexpr uint32_t CAPACITY{1 * 1024 * 1024};
  CircularBuffer buffer{CAPACITY};

  vector<char> data(CAPACITY, 'a');
  buffer.Write(data);
  EXPECT_EQ(0, buffer.GetFreeSize());
  EXPECT_EQ(CAPACITY, buffer.GetCapacity());
  EXPECT_EQ(CAPACITY, buffer.GetSize());

  uint32_t total_size = CAPACITY;

  while (total_size) {
    EXPECT_EQ(CAPACITY - total_size, buffer.GetFreeSize());
    EXPECT_EQ(total_size, buffer.GetSize());

    const uint32_t random_size = static_cast<uint32_t>(256 * (std::rand() / (float)RAND_MAX));
    const uint32_t read_size = std::min(random_size, buffer.GetSize());

    const vector<char> data = buffer.Read(read_size);
    EXPECT_EQ(read_size, data.size());
    total_size -= read_size;

    EXPECT_EQ(total_size, buffer.GetSize());
  }
  
  EXPECT_EQ(CAPACITY, buffer.GetFreeSize());
  EXPECT_EQ(CAPACITY, buffer.GetCapacity());
  EXPECT_EQ(0, buffer.GetSize());
}

TEST(status, clear)
{
  CircularBuffer buffer{13};

  vector<char> data(7, 'a');
  buffer.Write(data);
  EXPECT_NE(0, buffer.GetSize());

  EXPECT_NO_THROW({
    buffer.Clear();
  });

  EXPECT_EQ(0, buffer.GetSize());
  EXPECT_EQ(buffer.GetCapacity(), buffer.GetFreeSize());
}

TEST(status, is_empty)
{
  CircularBuffer buffer{13};

  try {
    EXPECT_TRUE(buffer.IsEmpty());
  } catch (const std::exception& excp) {
    std::cout << excp.what() << std::endl;
    FAIL();
  }

  vector<char> data(7, 'a');
  buffer.Write(data);
  
  try {
    EXPECT_FALSE(buffer.IsEmpty());
  } catch (const std::exception& excp) {
    std::cout << excp.what() << std::endl;
    FAIL();
  }
}

TEST(status, is_full)
{
  constexpr auto BUFFER_SIZE{13};
  CircularBuffer buffer{BUFFER_SIZE};

  try {
    EXPECT_FALSE(buffer.IsFull());
  } catch (const std::exception& excp) {
    std::cout << excp.what() << std::endl;
    FAIL();
  }

  vector<char> data(7, 'a');
  buffer.Write(data);
  
  try {
    EXPECT_FALSE(buffer.IsFull());
  } catch (const std::exception& excp) {
    std::cout << excp.what() << std::endl;
    FAIL();
  }

  data.resize(BUFFER_SIZE - data.size());
  buffer.Write(data);

  try {
    EXPECT_TRUE(buffer.IsFull());
  } catch (const std::exception& excp) {
    std::cout << excp.what() << std::endl;
    FAIL();
  }

  EXPECT_EQ(0, buffer.GetFreeSize());
  EXPECT_EQ(BUFFER_SIZE, buffer.GetSize());
}

TEST(multithread, write)
{
  constexpr uint32_t WRITE_SIZE[]{74, 53};
  constexpr auto WRITE_COUNT{100000};
  constexpr auto BUFFER_SIZE{WRITE_COUNT * (WRITE_SIZE[0] + WRITE_SIZE[1])};
  CircularBuffer buffer{BUFFER_SIZE};

  EXPECT_EQ(0, buffer.GetSize());
  EXPECT_EQ(BUFFER_SIZE, buffer.GetFreeSize());

  const auto write_process = [](CircularBuffer& buffer, const uint32_t write_size, uint32_t write_count)
  {
    const vector<char> cache(write_size);

    while (write_count--) {
      buffer.Write(cache);
    }
  };

  constexpr uint32_t THREADS_COUNT{2};
  thread write_threads[THREADS_COUNT];

  for (uint32_t index = 0; index < THREADS_COUNT; ++index) {
    write_threads[index] = thread(write_process, std::ref(buffer), WRITE_SIZE[index], WRITE_COUNT);
  }

  for (uint32_t index = 0; index < THREADS_COUNT; ++index) {
    if (write_threads[index].joinable()) {
      write_threads[index].join();
    }
  }

  EXPECT_EQ(BUFFER_SIZE, buffer.GetSize());
  EXPECT_EQ(0, buffer.GetFreeSize());
}

TEST(multithread, read)
{
  constexpr uint32_t READ_SIZE[]{74, 53};
  constexpr auto READ_COUNT{10000};
  constexpr auto BUFFER_SIZE{READ_COUNT * (READ_SIZE[0] + READ_SIZE[1])};
  CircularBuffer buffer{BUFFER_SIZE};

  {
    vector<char> fill_buffer(BUFFER_SIZE);
    buffer.Write(fill_buffer);
  }

  EXPECT_EQ(BUFFER_SIZE, buffer.GetSize());
  EXPECT_EQ(0, buffer.GetFreeSize());

  const auto read_process = [](CircularBuffer& buffer, const uint32_t read_size, uint32_t read_count)
  {
    while (read_count--)
      const auto cache = buffer.Read(read_size);
  };

  constexpr uint32_t THREADS_COUNT{2};
  thread read_threads[THREADS_COUNT];

  for (uint32_t index = 0; index < THREADS_COUNT; ++index)
    read_threads[index] = thread(read_process, std::ref(buffer), READ_SIZE[index], READ_COUNT);

  for (uint32_t index = 0; index < THREADS_COUNT; ++index) {
    if (read_threads[index].joinable())
      read_threads[index].join();
  }

  EXPECT_EQ(0, buffer.GetSize());
  EXPECT_EQ(BUFFER_SIZE, buffer.GetFreeSize());
}

TEST(multithread, verification)
{
  // constexpr auto TRANSFER_SIZE{1 * 1024 * 1024 * 1024};
  constexpr auto TRANSFER_SIZE{32 * 1024};
  constexpr auto BUFFER_SIZE{32 * 1024};
  CircularBuffer buffer{BUFFER_SIZE};

  thread write_thread = thread([&buffer, &TRANSFER_SIZE]()
  {
    auto ransfer_size{TRANSFER_SIZE};
    char counter{0};
    vector<char> cache(32);

    while (ransfer_size) {
      for (auto& itr : cache) {
        itr = counter;
        ++counter;
      }

      buffer.Write(cache);
      ransfer_size -= cache.size();
    }
  });

  thread read_thread = thread([&buffer, &TRANSFER_SIZE]()
  {
    auto ransfer_size{TRANSFER_SIZE};
    char counter{0};
    const auto READ_SIZE{32 * 2};
    uint32_t read_size{0};

    while (ransfer_size) {
      while (buffer.GetSize() < READ_SIZE)
        sleep_for(microseconds(1));

      const auto cache = buffer.Read(READ_SIZE);

      for (auto& itr : cache) {
        EXPECT_EQ(itr, counter);
        ++counter;
      }

      read_size += cache.size();
      ransfer_size -= cache.size();
    }

    EXPECT_EQ(read_size, TRANSFER_SIZE);
  });

  if (write_thread.joinable())
    write_thread.join();
  if (read_thread.joinable())
    read_thread.join();

  EXPECT_EQ(0, buffer.GetSize());
  EXPECT_EQ(BUFFER_SIZE, buffer.GetFreeSize());
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}