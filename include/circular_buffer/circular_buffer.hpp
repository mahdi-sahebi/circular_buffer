#ifndef ELB_CIRCULAR_BUFFER_H_
#define ELB_CIRCULAR_BUFFER_H_

#include <cstdbool>
#include <cstdint>
#include <vector>

namespace ELB
{

class CircularBuffer
{
public:
  CircularBuffer() = delete;
  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer(const uint32_t capacity);
  ~CircularBuffer();
  uint32_t GetCapacity();
  uint32_t GetFreeSize();
  bool IsEmpty();
  bool IsFull();
  std::vector<char> Read(const uint32_t size);
  void Write(const std::vector<char>& data);
// TODO(MN): Tests of is_empty/is_full
private:
  
};

}


#endif /* ELB_CIRCULAR_BUFFER_H_ */