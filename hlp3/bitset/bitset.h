/**
 * @file bitset.h
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief assignment 2
 * @version 0.1
 * @date 2023-09-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BITSET_H
#define BITSET_H
#include <exception>
#include <string>
namespace HLP3 {
    
// TO DO [see specs]
    template <size_t bitsize>
    class bitset{
    private: 
        char* bits{};
    public:
        bitset();
        ~bitset();
        void set(size_t pos, bool value = true);
        void reset(size_t pos);
        void flip(size_t pos);
        bool test(size_t pos) const;
        size_t count() const;
        size_t size() const;
        bool operator[](size_t pos) const;
        std::string to_string(char f = '0', char t = '1');
    };
}

#include "bitset.hpp"

#endif

