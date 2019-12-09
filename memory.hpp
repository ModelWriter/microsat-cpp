//  ----------------------------------------------------------------------------
//  Header file for the Memory class.        memory.hpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 09, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#ifndef MICROSAT_MEMORY_HPP
#define MICROSAT_MEMORY_HPP

#include "tools.hpp"
#include <iostream>
#include <vector>

namespace microsat {

template <typename T> class FixedAllocator;
// -----------------------------------------------------------------------------
//
template <typename T> class Memory {

  private:
    const std::size_t max;
    T* memory;
    std::size_t used = 0;

  public:
    using value_type = T;

    explicit Memory(int mem_max) noexcept : max(mem_max), memory(new int[max]) {
        PRINT("Memory::mem_max " << mem_max * sizeof(T) << " bytes");
    }

    ~Memory() {
        PRINT("Memory::~Memory()");
        delete[] memory;
    }

    [[nodiscard]] int mem_used() const { return used; } // make it const later

    [[nodiscard]] int mem_max() const { return max; }

    // -------------------------------------------------------------------------
    // allocates memory for pointers
    value_type* allocate(std::size_t mem_size) {
        PRINT("Allocator::allocate " << mem_size * sizeof(T) << " bytes");
        // In case the code is used within a code base
        if (used + mem_size > max) {
            throw std::runtime_error("Memory::allocate out of memory");
        }
        // Compute a pointer to the new memory location
        value_type* store = memory + used;
        used += mem_size; // Update the size of the used
        PRINT("Allocator::used " << used);
        return store;
    }

    // -------------------------------------------------------------------------
    // makes a vector from fixed allocated array by copying its members
    static auto make_vector(value_type* tp, std::size_t size) {
        std::vector<value_type, FixedAllocator<value_type>> vector{
            FixedAllocator<value_type>{tp, size}};
        vector.reserve(size);
        std::copy(tp, tp + size, std::back_inserter(vector));
        return vector;
    }

    // -------------------------------------------------------------------------
    // creates an empty vector from fixed allocated array
    static auto create_vector(value_type* tp, std::size_t size) {
        std::vector<value_type, FixedAllocator<value_type>> vector{
            FixedAllocator<value_type>{tp, size}};
        vector.reserve(size);
        return vector;
    }

    void resize(std::size_t size) { used = size; };
    value_type* get_raw_memory() { return memory; }
};

// -----------------------------------------------------------------------------
// Normal Allocator
template <typename T> class Allocator {
  private:
    Memory<T>& memory;

  public:
    using value_type = T;

    explicit Allocator(Memory<T>& memory) : memory(memory){};

    ~Allocator() { PRINT("Allocator::~Allocator()"); }

    value_type* allocate(std::size_t mem_size) {
        return memory.allocate(mem_size);
    }

    void deallocate(value_type* p, std::size_t n) noexcept {
        PRINT("Allocator::deallocate " << n * sizeof *p << " bytes");
    }

    [[nodiscard]] std::size_t max_size() const noexcept {
        return memory.mem_max();
    }
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
// Fixed Allocator
template <typename T> class FixedAllocator {

  private:
    T* memory; // holding a share pointer
    std::size_t max = 0;

  public:
    using value_type = T;

    explicit FixedAllocator(T* memory, std::size_t max_size)
        : memory(memory), max(max_size){};

    ~FixedAllocator() { PRINT("FixedAllocator::~FixedAllocator()"); }

    value_type* allocate(std::size_t mem_size) { return memory; }

    void deallocate(value_type* p, std::size_t n) noexcept {
        PRINT("FixedAllocator::deallocate " << n * sizeof *p << " bytes");
    }

    [[nodiscard]] std::size_t max_size() const noexcept { return max; }
};

template <typename T, typename U>
bool operator==(const FixedAllocator<T>&, const FixedAllocator<U>&) {
    return true;
}

template <typename T, typename U>
bool operator!=(const FixedAllocator<T>& x, const FixedAllocator<U>& y) {
    return !(x == y);
}

} // namespace microsat

#endif // MICROSAT_MEMORY_HPP
