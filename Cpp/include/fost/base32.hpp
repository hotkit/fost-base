/**
    Copyright 2015-2019, Proteus Tech Co Ltd. <http://www.kirit.com/Rask>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <fost/array>
#include <fost/string>
#include <fost/exception/parse_error.hpp>


namespace fostlib {


    /// Encoding and checking of the base32 strings. We will ignore the nil at
    /// the end
    //. of the alphabet
    template<const char A[34]>
    struct base32_string_tag {
        static constexpr const char *const characters = A;

        static void do_encode(fostlib::nliteral l, fostlib::ascii_string &s) {
            s = l;
            check_encoded(s);
        }
        static void do_encode(
                const fostlib::ascii_string &l, fostlib::ascii_string &s) {
            s = l;
            check_encoded(s);
        }
        static void check_encoded(const fostlib::ascii_string &s) {
            if (s.underlying().find_first_not_of(characters)
                != std::string::npos)
                throw fostlib::exceptions::parse_error(
                        "Non base32 character found in string",
                        coerce<string>(s));
        }
    };


    /// Base32 alphabet from RFC
    constexpr const char base32_alphabet[34] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=";
    typedef fostlib::tagged_string<
            base32_string_tag<base32_alphabet>,
            fostlib::ascii_string>
            base32_string;
    /// Base32 hex alphabet from RFC
    constexpr const char base32hex_alphabet[34] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUV=";
    typedef fostlib::tagged_string<
            base32_string_tag<base32hex_alphabet>,
            fostlib::ascii_string>
            base32hex_string;
    /// Crockford's base 32 alphabet. Omits i, l,  o and u.
    constexpr const char base32c_alphabet[34] =
            "0123456789abcdefghjkmnpqrstvwxyz=";
    typedef tagged_string<base32_string_tag<base32c_alphabet>, ascii_string>
            base32c_string;


    namespace detail {


        /// Encode up to 5 bytes into the provided string. Pad if less than 5
        /// bytes are provided
        std::string &encode_b32_5bytes(
                const char[34], std::string &, array_view<const unsigned char>);

        /// Decode up to 8 characters of input and return up to 5 bytes of output
        std::pair<std::array<unsigned char, 5>, std::size_t>
                decode_b32_5bytes(const char alphabet[34], f5::u8view a);


    }


    /// Convert a byte array to base32
    template<const char A[34]>
    struct coercer<
            tagged_string<base32_string_tag<A>, ascii_string>,
            array_view<const unsigned char>> {
        tagged_string<base32_string_tag<A>, ascii_string>
                coerce(array_view<const unsigned char> m) {
            std::string into;
            detail::encode_b32_5bytes(A, into, m);
            return tagged_string<base32_string_tag<A>, ascii_string>{fostlib::string{std::move(into)}};
        }
    };

    /// Convert a byte array to base32
    template<const char A[34], std::size_t N>
    struct coercer<
            tagged_string<base32_string_tag<A>, ascii_string>,
            std::array<f5::byte, N>> {
        tagged_string<base32_string_tag<A>, ascii_string>
                coerce(std::array<f5::byte, N> m) {
            return fostlib::coerce<
                    tagged_string<base32_string_tag<A>, ascii_string>>(
                    array_view<const unsigned char>(
                            reinterpret_cast<const unsigned char *>(m.data()),
                            m.size()));
        }
    };

    /// Convert lstring to base32
    template<const char A[34]>
    struct coercer<tagged_string<base32_string_tag<A>, ascii_string>, f5::lstring> {
        tagged_string<base32_string_tag<A>, ascii_string> coerce(f5::lstring m) {
            return fostlib::coerce<
                    tagged_string<base32_string_tag<A>, ascii_string>>(
                    array_view<const unsigned char>(m));
        }
    };


    /// Convert base32 to a vector of bytes
    template<const char A[34]>
    struct coercer<
            std::vector<unsigned char>,
            tagged_string<base32_string_tag<A>, ascii_string>> {
        std::vector<unsigned char> coerce(
                const tagged_string<base32_string_tag<A>, ascii_string> &s) {
            f5::u8view b{s};
            std::vector<unsigned char> v;
            for (auto d = detail::decode_b32_5bytes(A, b); d.second;
                 b = b.substr(8), d = detail::decode_b32_5bytes(A, b)) {
                v.insert(v.end(), d.first.begin(), d.first.begin() + d.second);
            }
            return v;
        }
    };


}
