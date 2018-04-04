/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/base32.hpp>


std::string &fostlib::detail::encode_b32_5bytes(
    const char alphabet[33], std::string &into, array_view<const unsigned char> bytes
) {
    if ( bytes.size() == 0 ) return into;
    into += alphabet[(bytes[0] >> 3) & 0x1f];
    uint16_t buffer = (bytes[0] & 0x7);
    std::size_t bits{3};
    std::size_t byte{1};
    while ( byte < bytes.size() ) {
        if ( bits < 5 ) {
            buffer = (buffer << 8) | bytes[byte];
            ++byte; bits += 8;
        }
        /// Consume the bits we can
        const std::size_t remainder{bits - 5};
        into += alphabet[(buffer >> remainder) & 0x1f];
        bits = remainder;
    }
    switch ( bits ) {
    case 3:
        into += alphabet[(buffer << 2) & 0x1f];
        into += "======";
        break;
    case 4:
        into += alphabet[(buffer << 1) & 0x1f];
        into += "===";
        break;
    case 5:
        into += alphabet[buffer & 0x1f];
        break;
    case 6:
        into += alphabet[(buffer >> 1) & 0x1f];
        into += alphabet[(buffer << 4) & 0x1f];
        into += "====";
        break;
    case 7:
        into += alphabet[(buffer >> 2) & 0x1f];
        into += alphabet[(buffer << 3) & 0x1f];
        into += '=';
        break;
    default:
        throw fostlib::exceptions::not_implemented(__func__, "Left over bits", bits);
    }
    return into;
}

