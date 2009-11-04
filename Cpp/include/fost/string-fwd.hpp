/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_FWD_HPP
#define FOST_STRING_FWD_HPP
#pragma once


#include <fost/config.hpp>
#include <string>


namespace fostlib {


    namespace utf {

        template< std::size_t W > struct compiler_wide_character_traits;

        template<> struct compiler_wide_character_traits< 2 > {
            typedef unsigned char utf8;
            typedef wchar_t utf16;
            typedef uint32_t utf32;

            typedef const char * nliteral;
            typedef const wchar_t * wliteral;

            typedef wchar_t native_char;
            typedef const char * char_literal;
            typedef char_literal non_native_literal;
            typedef const wchar_t * native_literal;
            typedef std::wstring native_string;
            typedef std::string non_native_string;

            typedef std::wostream ostream;
            typedef std::wstringstream stringstream;
        };
        template<> struct compiler_wide_character_traits< 4 > {
            typedef unsigned char utf8;
            typedef uint16_t utf16;
            typedef wchar_t utf32;

            typedef const char * nliteral;
            typedef const wchar_t * wliteral;

            typedef char native_char;
            typedef const char * char_literal;
            typedef wliteral non_native_literal;
            typedef const char * native_literal;
            typedef std::string native_string;
            typedef std::wstring non_native_string;

            typedef std::ostream ostream;
            typedef std::stringstream stringstream;
        };

        const std::size_t wchar_t_width = sizeof( wchar_t );
    }

    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::utf8 utf8;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::utf16 utf16;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::utf32 utf32;

    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::nliteral nliteral;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::wliteral wliteral;

    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::char_literal char_literal;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::non_native_literal non_native_literal;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::native_char native_char;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::native_literal native_literal;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::native_string native_string;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::non_native_string non_native_string;

    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::ostream ostream;
    typedef utf::compiler_wide_character_traits< utf::wchar_t_width >::stringstream stringstream;

    class string;


}


#endif // FOST_STRING_FWD_HPP
