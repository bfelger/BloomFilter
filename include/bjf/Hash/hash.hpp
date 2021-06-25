///////////////////////////////////////////////////////////////////////////////
// hash.hpp
///////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef BJF__HASH_HPP
#define BJF__HASH_HPP

#include <cstddef>
#include <cstdint>

namespace bjf::Hash {

#if defined(_WIN64) || defined(__x86_64__)
#define HASH_64
    using hash_t = std::uint64_t;
#else
    using hash_t = std::uint32_t;
#endif

    constexpr std::size_t hash_size = sizeof(hash_t);
}

#endif // !BJF__HASH_HPP