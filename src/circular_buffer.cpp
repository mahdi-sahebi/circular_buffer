#include <algorithm>
#include <stdexcept>
#include "circular_buffer/circular_buffer.hpp"

using namespace std;

namespace ELB
{

  CircularBuffer::CircularBuffer(const uint32_t capacity) :
    capacity_{capacity}, read_index_{0}, write_index_{0}, free_size_{capacity}
  {
    if (0 == capacity)
      throw std::invalid_argument("[CircularBuffer] Zero capacity allocation");

    buffer_.resize(capacity);

    if (buffer_.size() < capacity)
      throw bad_alloc();
  }

  CircularBuffer::~CircularBuffer()
  {
  }

  uint32_t CircularBuffer::GetCapacity() const
  {
    return capacity_;
  }

  uint32_t CircularBuffer::GetWriteIndex()
  {
    lock_guard<recursive_mutex> guard(mutex_);
    return write_index_;
  }

  uint32_t CircularBuffer::GetReadIndex()
  {
    lock_guard<recursive_mutex> guard(mutex_);
    return read_index_;
  }

  uint32_t CircularBuffer::GetSize()
  {
    lock_guard<recursive_mutex> guard(mutex_);

    return capacity_ - free_size_;
  }

  uint32_t CircularBuffer::GetFreeSize()
  {
    lock_guard<recursive_mutex> guard(mutex_);

    return free_size_;
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
    lock_guard<recursive_mutex> guard(mutex_);

    write_index_ = 0;
    read_index_ = 0;
    free_size_ = capacity_;
  }

  std::vector<char> CircularBuffer::Read(const uint32_t size)
  {
    if (size > GetSize()) {
      throw out_of_range("[CircularBuffer] Not enough data to read");
    }

    const uint32_t data_size = size;
    vector<char> data;// TODO(MN): Don't insert. resize at the creation time. copy then. return move

    // TODO(MN): Don't define variables of two parts
    unique_lock<recursive_mutex> guard(mutex_);
    const uint32_t first_part_size = min(capacity_ - read_index_, data_size);
    data.insert(cend(data), cbegin(buffer_) + read_index_, cbegin(buffer_) + read_index_ + first_part_size);
    read_index_ += first_part_size;

    const uint32_t second_part_size = data_size - first_part_size;
    if (second_part_size) {
      data.insert(cend(data), cbegin(buffer_), cbegin(buffer_) + second_part_size);
      read_index_ = (read_index_ + second_part_size) % capacity_;
    }

    free_size_ += data_size;

    return data;
  }

  void CircularBuffer::Write(const std::vector<char>& data)
  {
    const uint32_t data_size = data.size();
    if (data_size > GetFreeSize()) {
      throw overflow_error("[CircularBuffer] Not enough space to write");
    }

    unique_lock<recursive_mutex> guard(mutex_);
    const uint32_t first_part_size = min(capacity_ - write_index_, data_size);
    copy(begin(data), begin(data) + first_part_size, begin(buffer_) + write_index_);
    write_index_ += first_part_size;

    const uint32_t second_part_size = data_size - first_part_size;
    if (second_part_size) {
      copy(begin(data) + first_part_size, end(data), begin(buffer_));
      write_index_ = (write_index_ + second_part_size) % capacity_;
    }

    free_size_ -= data_size;
  }
}
