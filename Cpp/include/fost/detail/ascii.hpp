/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_ASCII_STRING_HPP
#define FOST_ASCII_STRING_HPP


#include <fost/detail/tagged-string.hpp>


namespace fostlib {


    struct FOST_CORE_DECLSPEC ascii_string_tag {
        static void do_encode( fostlib::nliteral from, std::string &into );
        static void do_encode( const std::string &from, std::string &into );
        static void check_encoded( const std::string &s );
    };
    typedef tagged_string< ascii_string_tag, std::string > ascii_string;

    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_string, std::vector< ascii_string::value_type > > {
        ascii_string coerce( const std::vector< ascii_string::value_type > &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, ascii_string > {
        string coerce( const ascii_string &s );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_string, string > {
        ascii_string coerce( const string &s );
    };
    template<>
    struct coercer< std::string, ascii_string > {
        const std::string &coerce( const ascii_string &s ) {
            return s.underlying();
        }
    };
    template<>
    struct coercer< ascii_string, std::string > {
        ascii_string coerce( const std::string &s ) {
            return ascii_string( s );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< std::wstring, ascii_string > {
        std::wstring coerce( const ascii_string &s );
    };


}


#endif // FOST_ASCII_STRING_HPP
