#include <algorithm>
#include <stdexcept>
#include "circular_buffer/circular_buffer.hpp"

using namespace std;

namespace ELB
{

  CircularBuffer::CircularBuffer(const uint32_t capacity) :
    read_index_{0}, write_index_{0}
  {
    if (0 == capacity)
      throw std::invalid_argument("[CircularBuffer] Zero capacity allocation");

    buffer_.resize(capacity);
  }

  CircularBuffer::~CircularBuffer()
  {
  }

  uint32_t CircularBuffer::GetCapacity()
  {
    return 0;
  }

  uint32_t CircularBuffer::GetFreeSize()
  {
    return 0;
  }

  bool CircularBuffer::IsEmpty()
  {
    return false;
  }

  bool CircularBuffer::IsFull()
  {
    return false;
  }

  std::vector<char> CircularBuffer::Read(const uint32_t size)
  {
    vector<char> data;
    return data;
  }

  void CircularBuffer::Write(const std::vector<char>& data)
  {
  }

}