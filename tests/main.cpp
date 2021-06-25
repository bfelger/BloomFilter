///////////////////////////////////////////////////////////////////////////////
// tests/main.cpp
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <bjf/BloomFilter/BloomFilter.hpp>

using namespace bjf;

static void print_bits(const BloomFilter::filter_t& bitvector) {
    for (BloomFilter::filter_t i = 0; i < BloomFilter::filter_bits; i++) {
        if ((bitvector & ((BloomFilter::filter_t)1 << i)) >> i)
            std::cout << "X";
        else
            std::cout << " ";
    }
    std::cout << "\n";
}

int main() {
    BloomFilter::filter_t super_filter1 = 0;
    BloomFilter::filter_t super_filter2 = 0;

    auto batman = BloomFilter::get_bitset("Batman");
    std::cout << "Batman:            ";
    print_bits(batman);

    auto superman = BloomFilter::get_bitset("Superman");
    std::cout << "Superman:          ";
    print_bits(superman);

    auto green_lantern = BloomFilter::get_bitset("Green Lantern");
    std::cout << "Green Lantern:     ";
    print_bits(green_lantern);

    super_filter1 = batman | superman | green_lantern;

    super_filter2 = BloomFilter::add_string_value(super_filter2, "Batman");
    super_filter2 = BloomFilter::add_string_value(super_filter2, "Superman");
    super_filter2 = BloomFilter::add_string_value(super_filter2, "Green Lantern");

    if (super_filter1 != super_filter2) {
        std::cout << "Filter sanity check failed:\nOrganic:           ";
        print_bits(super_filter1);
    }
    std::cout << "Aggregate:         ";
    print_bits(super_filter2);
    std::cout << "\n";

    auto wonder_woman = BloomFilter::get_bitset("Wonder Woman");
    std::cout << "Wonder Woman:      ";
    print_bits(wonder_woman);
    std::cout << "\n";

    // Test for inclusion
    if (BloomFilter::potentially_matches(super_filter1, "Batman"))
        std::cout << "Batman inclusion test passed.\n";
    else
        std::cout << "Batman inclusion test failed.\n";

    // Test for inclusion
    if (BloomFilter::potentially_matches(super_filter1, "Superman"))
        std::cout << "Superman inclusion test passed.\n";
    else
        std::cout << "Superman inclusion test failed.\n";

    // Test for inclusion
    if (BloomFilter::potentially_matches(super_filter1, "Green Lantern"))
        std::cout << "Green Lantern inclusion test passed.\n";
    else
        std::cout << "Green Lantern inclusion test failed.\n";

    // Test for exclusion (barring false positives)
    if (!BloomFilter::potentially_matches(super_filter1, "Wonder Woman"))
        std::cout << "Wonder Woman exclusion test passed.\n";
    else
        std::cout << "Wonder Woman exclusion test failed.\n";

    return 0;
}