#include <algorithm>
#include <stdexcept>
#include "circular_buffer/circular_buffer.hpp"

using namespace std;

namespace ELB
{

  CircularBuffer::CircularBuffer(const uint32_t capacity) :
    read_index_{0}, write_index_{0}, capacity_{capacity}
  {
    if (0 == capacity)
      throw std::invalid_argument("[CircularBuffer] Zero capacity allocation");

    buffer_.resize(capacity);

    if (capacity_ < capacity)
      throw bad_alloc();// TODO(MN): Test
  }

  CircularBuffer::~CircularBuffer()
  {
  }

  uint32_t CircularBuffer::GetCapacity() const
  {
    return capacity_;
  }

  uint32_t CircularBuffer::GetReadIndex()
  {
    lock_guard<recursive_mutex> guard(read_mutex_);
    return read_index_;
  }

  uint32_t CircularBuffer::GetSize()
  {
    write_mutex_.lock();
    const uint32_t write_index = write_index_; 
    write_mutex_.unlock();
    const uint32_t read_index = GetReadIndex();

    uint32_t size = write_index - read_index;

    if (write_index < read_index)
      size = capacity_ - write_index - read_index;

    return size;
  }

  uint32_t CircularBuffer::GetFreeSize()
  {
    return (capacity_ - GetSize());
  }

  bool CircularBuffer::IsEmpty()
  {
    return (capacity_ == GetFreeSize());
  }

  bool CircularBuffer::IsFull()
  {
    return (0 == GetFreeSize());
  }

  void CircularBuffer::Clear()
  {
    // TODO(MN): Thread safety
    write_index_ = 0;
    read_index_ = 0;
  }

  std::vector<char> CircularBuffer::Read(const uint32_t size)
  {
    const uint32_t data_size = size;

    if (size > GetSize())
      throw out_of_range("[CircularBuffer] Not enough data to read");

    // TODO(MN): Optimize return vector
    vector<char> data;

    lock_guard<recursive_mutex> guard(read_mutex_);
    const uint32_t first_part_size = min(capacity_ - read_index_, data_size);
    data.insert(cend(data), cbegin(buffer_) + read_index_, cbegin(buffer_) + read_index_ + first_part_size);
    read_index_ += first_part_size;

    const uint32_t second_part_size = data_size - first_part_size;
    if (second_part_size) {
      data.insert(cend(data), cbegin(buffer_), cbegin(buffer_) + second_part_size);
      read_index_ = (read_index_ + second_part_size) % capacity_;
    }

    return data;
  }

  void CircularBuffer::Write(const std::vector<char>& data)
  {
    const uint32_t data_size = data.size();

    if (data_size > GetFreeSize())
      throw overflow_error("[CircularBuffer] Not enough space to write");
   
    lock_guard<recursive_mutex> guard(write_mutex_);
    const uint32_t first_part_size = min(capacity_ - write_index_, data_size);
    copy(begin(data), begin(data) + first_part_size, begin(buffer_) + write_index_);
    write_index_ += first_part_size;

    const uint32_t second_part_size = data_size - first_part_size;
    if (second_part_size) {
      copy(begin(data) + first_part_size, end(data), begin(buffer_));
      write_index_ = (write_index_ + second_part_size) % capacity_;
    }
  }

}