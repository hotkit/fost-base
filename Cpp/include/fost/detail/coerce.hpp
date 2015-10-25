/*
    Copyright 2008-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <fost/coerce.hpp>
#include <fost/exception.hpp>


namespace fostlib {


    /// Convert a Boost error to JSON
    template<>
    struct coercer<json, boost::system::error_code> {
        json coerce(const boost::system::error_code &e) {
            return json(e.message().c_str());
        }
    };


    /// Coerce fostlib::exceptions::exception instances to a string
    template< typename E >
    struct coercer< string, E,
        typename boost::enable_if< boost::is_base_of< exceptions::exception, E > >::type
    > {
        string coerce( const E &e ) {
            fostlib::stringstream ss;
            e.printOn( ss );
            return string(ss.str());
        }
    };
    /// Coerce fostlib:;exceptions::exception instances to json
    template< typename E >
    struct coercer< json, E,
        typename boost::enable_if<
            boost::is_base_of<exceptions::exception, E>>::type>
    {
        json coerce( const E &e ) {
            return e.as_json();
        }
    };


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

    template< typename I >
    struct coercer< string, I,
        typename boost::enable_if< boost::is_integral< I > >::type
    > {
        string coerce( I i ) {
            return fostlib::coercer< string, int64_t >().coerce( i );
        }
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
    struct FOST_CORE_DECLSPEC coercer< string, std::vector< wchar_t > > {
        string coerce( const std::vector< wchar_t > &c );
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
    struct coercer< string, float > {
        string coerce( float f ) {
            return coercer< string, double >().coerce( f );
        }
    };


}
