#ifndef ELB_CIRCULAR_BUFFER_H_
#define ELB_CIRCULAR_BUFFER_H_

// TODO(NM): const functions. noexcept function

#include <cstdbool>
#include <cstdint>
#include <vector>
#include <mutex>


namespace ELB
{

class Buffer
{
public:
  virtual uint32_t GetSize() = 0;
  virtual uint32_t GetCapacity() const noexcept = 0;
  virtual uint32_t GetFreeSize() noexcept = 0;
  virtual bool IsEmpty() noexcept = 0;
  virtual bool IsFull() noexcept = 0;
  virtual void Clear() noexcept = 0;
  virtual std::vector<char> Read(const uint32_t size) = 0;
  virtual void Write(const std::vector<char>& data) = 0;
  virtual void Write(const char* const data, const uint32_t size) = 0;
};

class CircularBuffer : public Buffer
{
public:
  CircularBuffer() = delete;
  CircularBuffer(const CircularBuffer&) = delete;
  CircularBuffer(const uint32_t capacity);
  ~CircularBuffer();
  uint32_t GetSize() noexcept override;
  uint32_t GetCapacity() const noexcept override;
  uint32_t GetFreeSize() noexcept override;
  bool IsEmpty() noexcept override;
  bool IsFull() noexcept override;
  void Clear() noexcept override;
  std::vector<char> Read(const uint32_t size) override;
  void Write(const std::vector<char>& data) override;
  void Write(const char* const data, const uint32_t size) override;

private:
  std::vector<char> buffer_;
  const uint32_t capacity_;
  uint32_t read_index_;
  uint32_t write_index_;
  uint32_t free_size_;
  std::recursive_mutex mutex_;
  uint32_t GetReadIndex() noexcept;
  uint32_t GetWriteIndex() noexcept;
};

}


#endif /* ELB_CIRCULAR_BUFFER_H_ */
