///////////////////////////////////////////////////////////////////////////////
// BloomFilter.hpp
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef BJF__BLOOM_FILTER_HPP
#define BJF__BLOOM_FILTER_HPP

#include <string_view>

#include "../Hash/FNV1a.hpp"
#include "../Hash/Murmur3.hpp"

namespace bjf::BloomFilter {

#if defined(_WIN64) || defined(__x86_64__)
    using filter_t = std::uint64_t;
#else
    using filter_t = std::uint32_t;
#endif

    constexpr filter_t mult_ident = 1;
    constexpr int filter_bits = sizeof(filter_t) * 8;
    constexpr int filter_bits_mask = filter_bits - 1;

    constexpr filter_t add_hash_value(const filter_t& filter, const bjf::Hash::hash_t& value)
    {
        return filter | 1ull << (value & filter_bits_mask);
    }

    constexpr filter_t get_bitset(const char* str, const std::size_t& len)
    {
        auto fnv = bjf::Hash::fnv1a_hash(str, len);
        auto mm3 = bjf::Hash::murmur3_hash(str, len, 47);

        auto filter = 1ull << (fnv & filter_bits_mask);
        filter |= 1ull << (mm3.v1 & filter_bits_mask);
        if constexpr(bjf::Hash::mm3_chunk_size == 2)
            filter |= 1ull << (mm3.v2 & filter_bits_mask);

        return filter;
    }

    constexpr filter_t get_bitset(std::string_view str)
    {
        return get_bitset(str.data(), str.size());
    }

    constexpr filter_t add_string_value(const filter_t& filter, const char* str, const std::size_t& len)
    {
        return filter | get_bitset(str, len);
    }

    constexpr filter_t add_string_value(const filter_t& filter, std::string_view str)
    {
        return filter | get_bitset(str.data(), str.size());
    }

    constexpr bool potentially_matches(const filter_t& filter, const filter_t& candidate)
    {
        return (filter & candidate) == candidate;
    }

    constexpr bool potentially_matches(const filter_t& filter, std::string_view str)
    {
        auto bitset = get_bitset(str.data(), str.size());
        return potentially_matches(filter, bitset);
    }
}

#endif // !BJF__BLOOM_FILTER_HPP
