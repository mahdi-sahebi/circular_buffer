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
    return buffer_.size();
  }

  uint32_t CircularBuffer::GetSize()
  {
    uint32_t size = 0;

    if (write_index_ >= read_index_)
      size = write_index_ - read_index_;
    else
      size = buffer_.size() - write_index_ - read_index_;

    return size;
  }

  uint32_t CircularBuffer::GetFreeSize()
  {
    return (buffer_.size() - GetSize());
  }

  bool CircularBuffer::IsEmpty()
  {
    return (buffer_.size() == GetFreeSize());
  }

  bool CircularBuffer::IsFull()
  {
    return (0 == GetFreeSize());
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