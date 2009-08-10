/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_BASE64_HPP
#define FOST_DETAIL_BASE64_HPP
#pragma once


#include <fost/detail/tagged-string.hpp>
#include <boost/array.hpp>
#include <boost/range/functions.hpp>


namespace fostlib {


    struct FOST_CORE_DECLSPEC base64_string_tag {
        static void do_encode( fostlib::nliteral from, ascii_string &into );
        static void do_encode( const ascii_string &from, ascii_string &into );
        static void check_encoded( const ascii_string &s );
    };
    typedef tagged_string< base64_string_tag, ascii_string > base64_string;

    namespace detail {
        base64_string base64_encode_3bytes( const unsigned char *data, std::size_t length );
    }


    template< std::size_t L >
    struct coercer< base64_string, unsigned char [L] > {
        base64_string coerce( const unsigned char pos[ L ] ) {
            fostlib::base64_string ret; std::size_t length = L;
            for ( ; length > 2; length -= 3, pos += 3 )
                ret += detail::base64_encode_3bytes( pos, 3 );
            return ret + detail::base64_encode_3bytes( pos, length );
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< base64_string, std::vector< unsigned char > > {
        base64_string coerce( const std::vector< unsigned char > &v );
    };

    template<>
    struct coercer< string, base64_string > {
        string coerce( const base64_string &h ) {
            return string(h.underlying().underlying());
        }
    };


}


#endif // FOST_DETAIL_BASE64_HPP
