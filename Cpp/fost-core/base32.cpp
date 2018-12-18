/**
    Copyright 2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/base32.hpp>


std::string &fostlib::detail::encode_b32_5bytes(
        const char alphabet[34],
        std::string &into,
        array_view<const unsigned char> bytes) {
    if (bytes.size() == 0) return into;
    into += alphabet[(bytes[0] >> 3) & 0x1f];
    uint16_t buffer = (bytes[0] & 0x7);
    std::size_t bits{3};
    std::size_t byte{1};
    while (byte < bytes.size()) {
        if (bits < 5) {
            buffer = (buffer << 8) | bytes[byte];
            ++byte;
            bits += 8;
        }
        /// Consume the bits we can
        const std::size_t remainder{bits - 5};
        into += alphabet[(buffer >> remainder) & 0x1f];
        bits = remainder;
    }
    switch (bits) {
    case 3:
        into += alphabet[(buffer << 2) & 0x1f];
        into += "======";
        break;
    case 4:
        into += alphabet[(buffer << 1) & 0x1f];
        into += "===";
        break;
    case 5: into += alphabet[buffer & 0x1f]; break;
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
        throw fostlib::exceptions::not_implemented(
                __func__, "Left over bits", bits);
    }
    return into;
}


std::pair<std::array<unsigned char, 5>, std::size_t>
        fostlib::detail::decode_b32_5bytes(
                const char alphabet[34], f5::u8view u8v) {
    if (not u8v.bytes()) return {};
    f5::cord::const_u8buffer a{u8v};
    std::array<unsigned char, 5> output{};
    std::size_t bytes{}, bits{}, pos{};
    uint16_t buffer{};
    do {
        if (a.size() <= pos || a[pos] == '=') {
            /// We're done, nothing left. Check padding
            switch (pos) {
            case 2:
                if (bytes != 1)
                    throw fostlib::exceptions::parse_error(
                            "Expected to have 1 byte with 2 characters of "
                            "input",
                            u8v);
                return std::make_pair(output, bytes);
            case 4:
                if (bytes != 2)
                    throw fostlib::exceptions::parse_error(
                            "Expected to have 2 bytes with 4 characters of "
                            "input",
                            u8v);
                return std::make_pair(output, bytes);
            case 5:
                if (bytes != 3)
                    throw fostlib::exceptions::parse_error(
                            "Expected to have 3 bytes with 5 characters of "
                            "input",
                            u8v);
                return std::make_pair(output, bytes);
            case 7:
                if (bytes != 4)
                    throw fostlib::exceptions::parse_error(
                            "Expected to have 4 bytes with 7 characters of "
                            "input",
                            u8v);
                return std::make_pair(output, bytes);
            default:
                throw fostlib::exceptions::parse_error(
                        "Incorrrect length or padding in base 32 string", u8v);
            }
        } else {
            buffer <<= 5;
            const auto ch = a[pos++];
            const std::size_t b =
                    std::find(alphabet, alphabet + 32, ch) - alphabet;
            buffer |= b;
            bits += 5;
        }
        if (bits >= 8) {
            bits -= 8;
            output[bytes++] = (buffer >> bits) & 0xff;
        }
    } while (bits);
    if (bytes != 5)
        throw fostlib::exceptions::parse_error(
                "Expected to have 5 bytes of output with 8 characters of "
                "input");
    return std::make_pair(output, bytes);
}
