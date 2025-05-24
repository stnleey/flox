# ArenaResource

`ArenaResource` is a custom bump allocator that implements the C++ polymorphic memory resource interface (`std::pmr::memory_resource`).  
It allows high-performance memory allocation from a fixed-size buffer with an optional upstream fallback.

## Purpose

To provide an efficient, low-latency allocator with predictable allocation patterns for performance-critical components.

## Class Definition

```cpp
class ArenaResource : public std::pmr::memory_resource {
public:
  ArenaResource(void *buffer, std::size_t size,
                std::pmr::memory_resource *upstream = std::pmr::null_memory_resource());

  void reset() noexcept;
};
```

## Responsibilities

- Allocates memory linearly from a pre-allocated buffer ("bump allocator")
- Falls back to upstream allocator if capacity is exceeded
- Does **not** support deallocation — memory is released only via `reset()`

## Internal Behavior

- `do_allocate(...)` aligns and reserves space within the buffer
- `do_deallocate(...)` is a no-op
- `reset()` resets the allocator to the beginning of the buffer

## Use Cases

- High-throughput components requiring many short-lived allocations
- Preallocated memory pools for real-time or embedded applications
- Unit testing or deterministic memory control

## Notes

- Thread-unsafe by design (intended for single-threaded access)
- Make sure the buffer remains valid for the allocator’s lifetime