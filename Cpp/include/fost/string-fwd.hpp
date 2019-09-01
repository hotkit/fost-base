/**
    Copyright 2008-2019 Red Anchor Trading Co. Ltd.

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

        /// Alias for the f5 UTF-8 view
        using u8_view [
                [deprecated("Replace fostlib::utf::u8_view with f5::u8view")]] =
                f5::cord::u8view;
    }

    /// The type for a single UTF-8 character. Up to four of these may be
    /// required for a single UCS4 code point
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::utf8 utf8;
    /// The type for a single UTF-16 character. Up to two of these may be
    /// required for a single UCS4 code point
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::utf16 utf16;
    /// A full UCS4 Unicode code point
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::utf32 utf32;

    /// This platform's narrow character literal type
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::nliteral
            nliteral;
    /// This platform's wide character literal type
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::wliteral
            wliteral;

    /// A character literal
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::char_literal
            char_literal;
    /// The type of character literal on platforms which are not like this one
    typedef utf::compiler_wide_character_traits<
            utf::wchar_t_width>::non_native_literal non_native_literal;
    /// The native character type
    typedef utf::compiler_wide_character_traits<utf::wchar_t_width>::native_char
            native_char;
    /// The native literal type
    typedef utf::compiler_wide_character_traits<
            utf::wchar_t_width>::native_literal native_literal;
    /// The native string type
    typedef utf::compiler_wide_character_traits<
            utf::wchar_t_width>::native_string native_string;
    /// The string type on platforms not like this one
    typedef utf::compiler_wide_character_traits<
            utf::wchar_t_width>::non_native_string non_native_string;

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
