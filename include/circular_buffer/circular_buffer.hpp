#ifndef ELB_CIRCULAR_BUFFER_H_
#define ELB_CIRCULAR_BUFFER_H_

// TODO(NM): const functions. noexcept function

#include <cstdbool>
#include <cstdint>
#include <vector>
#include <mutex>


namespace ELB
{

class CircularBuffer
{
public:
  CircularBuffer() = delete;
  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer(const uint32_t capacity);
  ~CircularBuffer();
  uint32_t GetSize();
  uint32_t GetCapacity() const;
  uint32_t GetFreeSize();
  bool IsEmpty();
  bool IsFull();
  void Clear();
  std::vector<char> Read(const uint32_t size);
  void Write(const std::vector<char>& data);
  void Write(const char* const data, const uint32_t size);

private:
  std::vector<char> buffer_;
  const uint32_t capacity_;
  uint32_t read_index_;
  uint32_t write_index_;
  uint32_t free_size_;
  std::recursive_mutex mutex_;
  uint32_t GetReadIndex();
  uint32_t GetWriteIndex();
};

}


#endif /* ELB_CIRCULAR_BUFFER_H_ */
