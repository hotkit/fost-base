/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_UNICODE_HPP
#define FOST_UNICODE_HPP
#pragma once


#include <fost/string-fwd.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>


namespace fostlib {


    namespace utf {


        const utf32 c_bom = 0xFEFF;
        const std::size_t utf32_utf8_max_length = 4;
        const std::size_t utf32_utf16_max_length = 2;

        // Checks that a given utf32 character is valid - throws an exception if it isn't
        FOST_CORE_DECLSPEC utf32 assertValid( utf32 codepoint );

        // Returns the number utf32 code points in the string
        FOST_CORE_DECLSPEC std::size_t length( nliteral );
        FOST_CORE_DECLSPEC std::size_t length( nliteral, nliteral );
        FOST_CORE_DECLSPEC std::size_t length( wliteral );
        FOST_CORE_DECLSPEC std::size_t length( wliteral, wliteral );

        // Return how many utf8/utf16 characters are needed for this code point or string
        FOST_CORE_DECLSPEC std::size_t utf8length( utf32 codepoint );
        FOST_CORE_DECLSPEC std::size_t utf16length( utf32 codepoint );
        FOST_CORE_DECLSPEC std::size_t native_length( utf32 codepoint );

        // The number of native characters to encode the length
        FOST_CORE_DECLSPEC std::size_t native_length( nliteral );
        FOST_CORE_DECLSPEC std::size_t native_length( wliteral );

        // Fetch the next utf32 character from the sequence (or throw an exception)
        FOST_CORE_DECLSPEC utf32 decode( nliteral begin, nliteral end );
        FOST_CORE_DECLSPEC utf32 decode( wliteral begin, wliteral end );
        FOST_CORE_DECLSPEC utf32 decode( wchar_t first, wchar_t second );
        FOST_CORE_DECLSPEC utf32 decode( wchar_t first );

        // Encode the character into the stream. Return the number of utf8/utf16 characters written (0 if the buffer isn't long enough)
        FOST_CORE_DECLSPEC std::size_t encode( utf32 codepoint, utf8 *begin, const utf8 *end );
        inline std::size_t encode( utf32 codepoint, char *begin, char *end ) {
            return encode( codepoint, reinterpret_cast< utf8 * >( begin ), reinterpret_cast< utf8 * >( end ) );
        }
        FOST_CORE_DECLSPEC std::size_t encode( utf32 codepoint, utf16 *begin, const utf16 *end );

        FOST_CORE_DECLSPEC string load_file( const boost::filesystem::wpath &filename );
        FOST_CORE_DECLSPEC string load_file( const boost::filesystem::wpath &filename, const string &default_content );

        FOST_CORE_DECLSPEC void save_file( const boost::filesystem::wpath &filename, const string &content );


    }


}


#endif // FOST_UNICODE_HPP

