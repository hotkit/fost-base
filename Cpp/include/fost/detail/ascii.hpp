/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_ASCII_STRING_HPP
#define FOST_ASCII_STRING_HPP


#include <fost/tagged-string.hpp>
#include <fost/json-core.hpp>


namespace fostlib {


    /// Tag type for ASCII strings
    struct FOST_CORE_DECLSPEC ascii_string_tag {
        static void do_encode( fostlib::nliteral from, std::string &into );
        static void do_encode( const std::string &from, std::string &into );
        static void check_encoded( const std::string &s );
    };
    /// ASCII strings are limited to the char range 1-127 (7 bit)
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
    template<>
    struct FOST_CORE_DECLSPEC coercer<json, ascii_string> {
        json coerce(const ascii_string &s);
    };


    /// Tag type for printable ASCII strings
    struct FOST_CORE_DECLSPEC ascii_printable_string_tag {
        static void do_encode( fostlib::nliteral from, std::string &into );
        static void do_encode( const std::string &from, std::string &into );
        static void check_encoded( const std::string &s );
    };
    /// ASCII printable strings are limited to the char range 32-127 (7 bit, excluding control characters)
    typedef tagged_string< ascii_printable_string_tag, std::string > ascii_printable_string;

    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_printable_string, std::vector< ascii_printable_string::value_type > > {
        ascii_printable_string coerce( const std::vector< ascii_printable_string::value_type > &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, ascii_printable_string > {
        string coerce( const ascii_printable_string &s );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_printable_string, string > {
        ascii_printable_string coerce( const string &s );
    };
    template<>
    struct coercer< std::string, ascii_printable_string > {
        const std::string &coerce( const ascii_printable_string &s ) {
            return s.underlying();
        }
    };
    template<>
    struct coercer< ascii_printable_string, std::string > {
        ascii_printable_string coerce( const std::string &s ) {
            return ascii_printable_string( s );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< std::wstring, ascii_printable_string > {
        std::wstring coerce( const ascii_printable_string &s );
    };

    template<>
    struct coercer<json, ascii_printable_string> {
        json coerce(const ascii_printable_string &s) {
            return f5::u8view{s.underlying()};
        }
    };
    template<>
    struct coercer<f5::u8view, ascii_printable_string> {
        f5::u8view coerce(const ascii_printable_string &s) {
            return f5::u8view{s.underlying()};
        }
    };


}


#endif // FOST_ASCII_STRING_HPP
