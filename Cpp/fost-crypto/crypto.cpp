/**
    Copyright 2015-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-crypto.hpp"
#include <fost/crypto.hpp>


const fostlib::module fostlib::c_fost_crypto(c_fost, "crypto");


bool fostlib::crypto_compare(
        fostlib::array_view<const unsigned char> left,
        fostlib::array_view<const unsigned char> right) {
    if (left.size() != right.size()) { return false; }
    unsigned char iored{0};
    for (std::size_t index{0}; index != left.size(); ++index) {
        iored |= (left[index] xor right[index]);
    }
    return iored == 0u;
}
