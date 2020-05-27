/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_STRING_FWD_HPP
#define FOST_STRING_FWD_HPP
#pragma once


#include <f5/cord/unicode.hpp>

#include <string>


namespace fostlib {


    namespace utf {

        /// Used to auto-detect the size of the wchar_t type
        template<std::size_t W>
        struct compiler_wide_character_traits;

        /// Concrete types where wchar_t is 2 bytes
        template<>
        struct compiler_wide_character_traits<2> {
            typedef unsigned char utf8;
            typedef wchar_t utf16;
            typedef uint32_t utf32;

            typedef const char *nliteral;
            typedef const wchar_t *wliteral;

            typedef wchar_t native_char;
            typedef const char *char_literal;
            typedef char_literal non_native_literal;
            typedef const wchar_t *native_literal;
            typedef std::wstring native_string;
            typedef std::string non_native_string;

            typedef std::wostream ostream;
            typedef std::wstringstream stringstream;
        };
        /// Concrete types where wchar_t is 4 bytes
        template<>
        struct compiler_wide_character_traits<4> {
            typedef unsigned char utf8;
            typedef uint16_t utf16;
            typedef wchar_t utf32;

            typedef const char *nliteral;
            typedef const wchar_t *wliteral;

            typedef char native_char;
            typedef const char *char_literal;
            typedef wliteral non_native_literal;
            typedef const char *native_literal;
            typedef std::string native_string;
            typedef std::wstring non_native_string;

            typedef std::ostream ostream;
            typedef std::stringstream stringstream;
        };

        /// The size of wchar_t on this platform
        const std::size_t wchar_t_width = sizeof(wchar_t);

    }

    /// The type for a single UTF-8 character. Up to four of these may be
    /// required for a single UCS4 code point
    using utf8 = unsigned char;
    /// The type for a single UTF-16 character. Up to two of these may be
    /// required for a single UCS4 code point
    using utf16 = uint16_t;
    /// A full UCS4 Unicode code point
    using utf32 = wchar_t;

    /// This platform's narrow character literal type
    using nliteral = char const *;
    /// This platform's wide character literal type
    using wliteral = wchar_t const *;

    /// A character literal
    using char_literal = char const *;
    /// The type of character literal on platforms which are not like this one
    using non_native_literal
            [[deprecated("Switch to using char16_t literals")]] =
                    wchar_t const *;
    /// The native character type
    using native_char [[deprecated("Switch to using char16_t literals")]] =
            utf::compiler_wide_character_traits<utf::wchar_t_width>::native_char;
    /// The native literal type
    using native_literal [[deprecated("Switch to using char16_t literals")]] =
            utf::compiler_wide_character_traits<
                    utf::wchar_t_width>::native_literal;
    /// The native string type
    using native_string [[deprecated("Switch to using char16_t literals")]] =
            utf::compiler_wide_character_traits<utf::wchar_t_width>::native_string;
    /// The string type on platforms not like this one
    using non_native_string
            [[deprecated("Switch to using char16_t literals")]] =
                    utf::compiler_wide_character_traits<
                            utf::wchar_t_width>::non_native_string;

    /// A suitable ostream type for this platform
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::ostream
            ostream;
    /// A suitable stringstream type for this platform
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::stringstream
            stringstream;

    // Forward declaration of string
    class string;


}


#endif // FOST_STRING_FWD_HPP
