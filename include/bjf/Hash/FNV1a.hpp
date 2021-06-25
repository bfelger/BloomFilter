///////////////////////////////////////////////////////////////////////////////
// FNV1a.hpp
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef BJF__FNV1A_HPP
#define BJF__FNV1A_HPP

#include "hash.hpp"

namespace bjf::Hash {

#ifdef HASH_64
    constexpr hash_t fnv_prime = 1099511628211;
    constexpr hash_t fnv_offset = 14695981039346656037;
#else // 32-bit
    constexpr hash_t fnv_prime = 16777619;
    constexpr hash_t fnv_offset = 2166136261;
#endif

    constexpr hash_t fnv1a_hash(const char* str, const std::size_t& len)
    {
        hash_t result = fnv_offset;
        for (int i = 0; i < len; i++) {
            result ^= str[i];
            result *= fnv_prime;
        }
        return result;
    }

}

#endif // !BJF__FNV1A_HPP