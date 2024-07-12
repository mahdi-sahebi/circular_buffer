#ifndef ELB_CIRCULAR_BUFFER_H_
#define ELB_CIRCULAR_BUFFER_H_

// TODO(MN): Tests of is_empty/is_full
// TODO(MN): Thread safety and its test, write/process/read - counter generator
// TODO(MN): Read/Write/GetFreeSize test for two part mode
// TODO(MN): Test of GetSize
// TODO(MN): Read/Write simple array APIs
// TODO(MN): Verify write/read APIs data

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

private:
  std::vector<char> buffer_;
  const uint32_t capacity_;
  uint32_t read_index_;
  uint32_t write_index_;
  std::recursive_mutex write_mutex_;
  std::recursive_mutex read_mutex_;
  uint32_t GetReadIndex();
};

}


#endif /* ELB_CIRCULAR_BUFFER_H_ */