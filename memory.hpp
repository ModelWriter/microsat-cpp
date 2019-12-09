//  ----------------------------------------------------------------------------
//  Header file for the Memory class.        memory.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 09, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef MICROSAT_MEMORY_HPP
#define MICROSAT_MEMORY_HPP

#include "tools.hpp"
#include <iostream>

namespace microsat {

template <typename T> class Allocator; // Forward Declaration
// -----------------------------------------------------------------------------
//
template <typename T> class Memory {
    friend class Allocator<T>;

  private:
    const std::size_t mem_max;
    T* memory;
    std::size_t used = 0;

  public:
    explicit Memory(int mem_max) noexcept
        : mem_max(mem_max), memory(new int[mem_max]) {
        PRINT("Memory::mem_max " << mem_max * sizeof(T) << " bytes");
    }

    ~Memory() {
        PRINT("Memory::~Memory()");
        delete[] memory;
    }

    [[nodiscard]] int mem_used() const { return used; }

    T* mem() const { return memory; }
};

template <typename T> class Allocator {
  private:
    Memory<T>& mem;

  public:
    using value_type = T;

    explicit Allocator(Memory<T>& memory) : mem(memory){};
    ~Allocator() { PRINT("Allocator::~Allocator()"); }

    value_type* allocate(std::size_t mem_size) { return getMemory(mem_size); }

    void deallocate(value_type* p, std::size_t n) noexcept {
        PRINT("Allocator::deallocate " << n * sizeof *p << " bytes");
    }

    value_type* getMemory(std::size_t mem_size) {
        PRINT("Allocator::allocate " << mem_size * sizeof(T) << " bytes");
        // In case the code is used within a code base
        if (mem.used + mem_size > mem.mem_max) {
            throw std::runtime_error("Allocator::allocate out of memory");
        }
        // Compute a pointer to the new memory location
        value_type* store = mem.memory + mem.used;
        mem.used += mem_size; // Update the size of the used
        PRINT("Allocator::used " << mem.used);
        return store;
    }

    [[nodiscard]] std::size_t max_size() const noexcept { return mem.mem_max; }
};

template <typename T, typename U>
bool operator==(const Allocator<T>&, const Allocator<U>&) {
    return true;
}

template <typename T, typename U>
bool operator!=(const Allocator<T>& x, const Allocator<U>& y) {
    return !(x == y);
}
// -----------------------------------------------------------------------------

} // namespace microsat

#endif // MICROSAT_MEMORY_HPP
