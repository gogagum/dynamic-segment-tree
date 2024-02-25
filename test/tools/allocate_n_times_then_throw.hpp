#ifndef ALLOCATE_N_TIMES_THEN_THROW
#define ALLOCATE_N_TIMES_THEN_THROW

#include <memory>
#include <stdexcept>

template <class T>
class AllocateNTimesThenThrow : std::allocator<T> {
 public:
  class Exception : public std::runtime_error {
   public:
    Exception() : std::runtime_error("Done all copies.") {
    }
  };

 public:
  AllocateNTimesThenThrow() = delete;
  AllocateNTimesThenThrow(const AllocateNTimesThenThrow&) = default;
  AllocateNTimesThenThrow(AllocateNTimesThenThrow&&) noexcept = default;
  explicit AllocateNTimesThenThrow(std::size_t counter)
      : counter_(std::make_shared<std::size_t>(counter)) {
  }

  AllocateNTimesThenThrow& operator=(const AllocateNTimesThenThrow&) = default;
  AllocateNTimesThenThrow& operator=(AllocateNTimesThenThrow&&) = default;

  T* allocate(std::size_t n);

  using std::allocator<T>::deallocate;
  ~AllocateNTimesThenThrow() = default;

 private:
  std::shared_ptr<std::size_t> counter_;
};

////////////////////////////////////////////////////////////////////////////////
template <class T>
inline T* AllocateNTimesThenThrow<T>::allocate(std::size_t n) {
  if (*counter_ > 0) {
    --*counter_;
    return std::allocator<T>::allocate(n);
  }
  throw Exception();
}

#endif  // ALLOCATE_N_TIMES_THEN_THROW
