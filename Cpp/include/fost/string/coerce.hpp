/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_COERCE_HPP
#define FOST_STRING_COERCE_HPP
#pragma once


#include <fost/coerce.hpp>
#include <fost/string.hpp>


namespace fostlib {


    template<>
    struct FOST_CORE_DECLSPEC coercer< string, t_null > {
        string coerce( t_null );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, bool > {
        string coerce( bool b );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< uint16_t, string > {
        uint16_t coerce( const string &s );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< int, string > {
        int coerce( const string &s );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, int > {
        string coerce( int i );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, unsigned int > {
        string coerce( unsigned int sz );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< int64_t, string > {
        int64_t coerce( const string &str );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, int64_t > {
        string coerce( int64_t i );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, uint64_t > {
        string coerce( uint64_t i );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< double, string > {
        double coerce( const string &w );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, double > {
        string coerce( double d );
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer< string, nliteral > {
        string coerce( nliteral str ) {
            return string( str );
        }
    };
    template< std::size_t L >
    struct FOST_CORE_DECLSPEC coercer< string, char[ L ] > {
        string coerce( const char str[ L ] ) {
            return string( str );
        }
    };
    template< std::size_t L >
    struct FOST_CORE_DECLSPEC coercer< string, const char[ L ] > {
        string coerce( const char str[ L ] ) {
            return string( str );
        }
    };
    template<>
    struct coercer< string, wliteral > {
        string coerce( wliteral str ) {
            return string( str );
        }
    };
    template< std::size_t L >
    struct FOST_CORE_DECLSPEC coercer< string, wchar_t[ L ] > {
        string coerce( const wchar_t str[ L ] ) {
            return string( str );
        }
    };
    template< std::size_t L >
    struct FOST_CORE_DECLSPEC coercer< string, const wchar_t[ L ] > {
        string coerce( const wchar_t str[ L ] ) {
            return string( str );
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< std::wstring, string > {
        std::wstring coerce( const string &str );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, std::wstring > {
        string coerce( const std::wstring &str );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, utf8string > {
        string coerce( const utf8string &str );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< utf8string, string > {
        utf8string coerce( const string &str );
    };

    template<>
    struct coercer< string, float > {
        string coerce( float f ) {
            return coercer< string, double >().coerce( f );
        }
    };


}


#endif // FOST_STRING_COERCE_HPP
