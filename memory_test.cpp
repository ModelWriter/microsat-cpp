//  ----------------------------------------------------------------------------
//  Header file for the memory_test class.        memory_test.cpp
//  Created by Ferhat Erata <ferhat.erata@yale.edu> on December 09, 2019.
//  Copyright (c) 2019 Yale University. All rights reserved.
// -----------------------------------------------------------------------------

#include "memory.hpp"
#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
#define ITERATIONS 50000
//-------------------------------------------------------------------
int main() {
    cout << "Testing lists with " << ITERATIONS << " numbers\n" << endl;
    std::cout << std::fixed;
    //-------------------------------------------------------------------
    // Test 1:  Array list
    {
        auto start = std::chrono::system_clock::now();
        int sz = ITERATIONS;
        int* array = new int[ITERATIONS * 5];
        for (int k = 0; k < sz; k++) {
            //            array[k] = rand();
            array[k] = 1; // Much time is spent in rand(); try with constants
        }
        delete[] array;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> seconds = end - start;
        cout << "Array list:  " << seconds.count() << "s" << endl;
    }

    //-------------------------------------------------------------------
    // Test 2: Preallocated vector<int>
    {
        auto start = std::chrono::system_clock::now();
        int sz = ITERATIONS;
        {
            vector<int> vec1;
            vec1.reserve(sz);
            for (int n = 0; n < sz; ++n)
                vec1.push_back(n);

            vector<int> vec2;
            vec2.reserve(sz);
            for (int n = 0; n < sz; ++n)
                vec2.push_back(n);

            vector<int> vec3;
            vec3.reserve(sz / 3);
            for (int n = 0; n < sz; ++n)
                vec3.push_back(n);

            vector<int> vec4;
            vec4.reserve(sz / 3);
            for (int n = 0; n < sz; ++n)
                vec4.push_back(n);

            vector<int> vec5;
            vec5.reserve(sz / 3);
            for (int n = 0; n < sz; ++n)
                vec5.push_back(n);

            vector<int> vec6;
            vec6.reserve(sz / 3);
            for (int n = 0; n < sz; ++n)
                vec6.push_back(n);

            vector<int> vec7;
            vec7.reserve(sz / 3);
            for (int n = 0; n < sz; ++n)
                vec7.push_back(n);

            vector<int> vec8;
            vec8.reserve(sz / 3);
            for (int n = 0; n < sz; ++n)
                vec8.push_back(n);
        }
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> secs = end - start;
        cout << "Preallocated vector<int>: " << secs.count() << "s" << endl;
    }

    {
        auto start = std::chrono::system_clock::now();
        int sz = ITERATIONS;
        using namespace microsat;
        Memory<int> memory{sz * 5};
        {
            std::vector<int, Allocator<int>> v1{Allocator<int>{memory}};
            v1.reserve(sz);
            for (int n = 1; n <= sz; ++n)
                v1.push_back(n);

            std::vector<int, Allocator<int>> v2{Allocator<int>{memory}};
            v2.reserve(sz);
            for (int n = 1; n <= sz; ++n)
                v2.push_back(n);

            std::vector<int, Allocator<int>> v3{Allocator<int>{memory}};
            v3.reserve(sz);
            for (int n = 1; n <= sz; ++n)
                v3.push_back(n * 3);

            std::vector<int, Allocator<int>> v4{Allocator<int>{memory}};
            v4.reserve(sz);
            for (int n = 1; n <= sz; ++n)
                v4.push_back(n * 3);

            std::vector<int, Allocator<int>> v5{Allocator<int>{memory}};
            v5.reserve(sz);
            for (int n = 1; n <= sz; ++n)
                v5.push_back(n * 3);

            //            v2.reserve(sz);
            //            for (int n = 1; n <= sz; ++n)
            //                v2.push_back(sz + n);

            //            for (int i = 0; i < memory.mem_used(); ++i) {
            //                std::cout << memory.mem()[i] << "\n";
            //            }
        }
        cout << "Memory::used " << memory.mem_used() << endl;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> secs = end - start;
        cout << "Custom allocated vector<int>: " << secs.count() << "s" << endl;
    }
}