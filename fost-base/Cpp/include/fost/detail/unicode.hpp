/*
    Copyright  2001-2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_UNICODE_HPP
#define FOST_UNICODE_HPP
#pragma once


#include <fost/string-fwd.hpp>

#include <boost/filesystem.hpp>


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


    /// Coerce a string to a file path
    template<>
    struct coercer< boost::filesystem::wpath, string > {
        boost::filesystem::wpath coerce( const string &s ) {
            return fostlib::coerce< std::wstring >( s );
        }
    };
    /// Coerce a file path to a string
    template<>
    struct coercer< string, boost::filesystem::wpath > {
        string coerce( const boost::filesystem::wpath &p ) {
#if (BOOST_VERSION_MAJOR < 46)
            return fostlib::coerce< string >( p.string() );
#else
            return fostlib::coerce< string >( p.wstring() );
#endif
        }
    };
    /// Coerce a file path to JSON
    template<>
    struct coercer< json, boost::filesystem::wpath > {
        json coerce( const boost::filesystem::wpath &p ) {
            return json( fostlib::coerce< string >( p ) );
        }
    };
    /// Coerce JSON to a file path
    template<>
    struct coercer< boost::filesystem::wpath, fostlib::json > {
        boost::filesystem::wpath coerce( const json &j ) {
            return boost::filesystem::wpath(
                fostlib::coerce< std::wstring >(
                    fostlib::coerce< string >( j )
                )
            );
        }
    };


}


namespace std {
    inline fostlib::ostream &operator << ( fostlib::ostream &o, const boost::filesystem::wpath &p ) {
        return o << fostlib::coerce< fostlib::string >( p );
    }
}


#endif // FOST_UNICODE_HPP

