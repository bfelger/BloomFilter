///////////////////////////////////////////////////////////////////////////////
// Murmur3.hpp
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef BJF__MURMUR3_HPP
#define BJF__MURMUR3_HPP

#include "hash.hpp"

namespace bjf::Hash {

    constexpr hash_t rotl(const hash_t& x, const uint8_t& r)
    {
        return (x << r) | (x >> (hash_size - r));
    }

    constexpr hash_t get_block(const hash_t* p, const int& i)
    {
        return p[i];
    }

#ifdef HASH_64

    constexpr hash_t fmix(const hash_t& k)
    {
        auto k2 = k;
        k2 ^= k2 >> 33;
        k2 *= 0xff51afd7ed558ccd;
        k2 ^= k2 >> 33;
        k2 *= 0xc4ceb9fe1a85ec53;
        k2 ^= k2 >> 33;

        return k2;
    }

    constexpr std::size_t mm3_block_size = 16; // Work 128-bits at a time.
    constexpr hash_t mm3_c1 = 0x87c37b91114253d5;
    constexpr hash_t mm3_c2 = 0x4cf5ad432745937f;

#else // 32-bit

    constexpr hash_t fmix(const hash_t& k)
    {
        k ^= k >> 16;
        k *= 0x85ebca6b;
        k ^= k >> 13;
        k *= 0xc2b2ae35;
        k ^= k >> 16;

        return k;
    }

    constexpr std::size_t mm3_block_size = 4; // Work 32-bits at a time.
    constexpr hash_t mm3_c1 = 0xcc9e2d51;
    constexpr hash_t mm3_c2 = 0x1b873593;

#endif

    struct mm3_result {
        hash_t v1;
#ifdef HASH_64
        hash_t v2;

        constexpr mm3_result(hash_t v1, hash_t v2) noexcept
            : v1{ v1 }, v2{ v2 }
        {}
#else
        constexpr mm3_result(hash_t v1) noexcept
            : v1{ v1 }
        {}
#endif
    };

    constexpr int mm3_chunk_size = mm3_block_size / hash_size;

    constexpr mm3_result murmur3_hash(const char* str, const std::size_t& len, const hash_t& seed)
    {
        const char* data = str;
        const std::size_t num_blocks = len / mm3_block_size;

        hash_t h1 = seed;
#ifdef HASH_64
        hash_t h2 = seed;
#endif

        const hash_t* blocks = (const hash_t*)(&data);

        for (int i = 0; i < num_blocks; i++) {
            hash_t k1 = get_block(blocks, i * mm3_block_size + 0);
#ifdef HASH_64
            hash_t k2 = get_block(blocks, i * mm3_block_size + 1);

            k1 *= mm3_c1;
            k1 = rotl(k1, 31);
            k1 *= mm3_c2;
            h1 ^= k1;

            h1 = rotl(h1, 27);
            h1 += h2;
            h1 = h1 * 5 + 0x52dce729;

            k2 *= mm3_c2;
            k2 = rotl(k2, 33);
            k2 *= mm3_c1;
            h2 ^= k2;

            h2 = rotl(h2, 31);
            h2 += h1;
            h2 = h2 * 5 + 0x38495ab5;
#else
            k1 *= c1;
            k1 = rotl(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = rotl(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
#endif
        }

        const char* tail = data + num_blocks * mm3_block_size;

        hash_t k1 = 0;
#ifdef HASH_64
        hash_t k2 = 0;
        const int rotl_shift = 33;
#else
        const int rotl_shift = 15;
#endif

        switch (len & (mm3_block_size - 1))
        {
#ifdef HASH_64
        case 15: k2 ^= ((hash_t)tail[14]) << 48;    [[fallthrough]];
        case 14: k2 ^= ((hash_t)tail[13]) << 40;    [[fallthrough]];
        case 13: k2 ^= ((hash_t)tail[12]) << 32;    [[fallthrough]];
        case 12: k2 ^= ((hash_t)tail[11]) << 24;    [[fallthrough]];
        case 11: k2 ^= ((hash_t)tail[10]) << 16;    [[fallthrough]];
        case 10: k2 ^= ((hash_t)tail[9]) << 8;      [[fallthrough]];
        case  9: k2 ^= ((hash_t)tail[8]) << 0;
            k2 *= mm3_c2;
            k2 = rotl(k2, 33);
            k2 *= mm3_c1;
            h2 ^= k2;                               [[fallthrough]];
        case  8: k1 ^= ((hash_t)tail[7]) << 56;     [[fallthrough]];
        case  7: k1 ^= ((hash_t)tail[6]) << 48;     [[fallthrough]];
        case  6: k1 ^= ((hash_t)tail[5]) << 40;     [[fallthrough]];
        case  5: k1 ^= ((hash_t)tail[4]) << 32;     [[fallthrough]];
        case  4: k1 ^= ((hash_t)tail[3]) << 24;     [[fallthrough]];
#endif
        case  3: k1 ^= ((hash_t)tail[2]) << 16;     [[fallthrough]];
        case  2: k1 ^= ((hash_t)tail[1]) << 8;      [[fallthrough]];
        case  1: k1 ^= ((hash_t)tail[0]) << 0;
            k1 *= mm3_c1;
            k1 = rotl(k1, rotl_shift);
            k1 *= mm3_c2;
            h1 ^= k1;
        };

        h1 ^= len;
#ifdef HASH_64
        h2 ^= len;

        h1 += h2;
        h2 += h1;
#endif

        h1 = fmix(h1);
#ifdef HASH_64
        h2 = fmix(h2);

        h1 += h2;
        h2 += h1;
#endif

#ifdef HASH_64
        return mm3_result{ h1, h2 };
#else
        return mm3_result{ h1 };
#endif
    }

}

#endif // !BJF__MURMUR3_HPP
