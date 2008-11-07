/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COERCE_WIN_HPP
#define FOST_COERCE_WIN_HPP
#pragma once


#include <fost/coerce.hpp>
#include <fost/null.hpp>


FOST_CORE_DECLSPEC bool operator ==( const _variant_t &l, fostlib::t_null );
inline bool operator ==( fostlib::t_null, const _variant_t &l ) {
    return l == fostlib::null;
}
inline bool operator !=( const _variant_t &l, fostlib::t_null ) {
    return !( l == fostlib::null );
}
inline bool operator !=( fostlib::t_null, const _variant_t &r ) {
    return !( r == fostlib::null );
}


namespace fostlib {


    template<>
    struct FOST_CORE_DECLSPEC coercer< string, HRESULT > {
        string coerce( HRESULT hr );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, _bstr_t > {
        string coerce( const _bstr_t &bstr );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< nullable< string >, _bstr_t > {
        nullable< string > coerce( const _bstr_t &bstr );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< _bstr_t, string > {
        _bstr_t coerce( const string &str );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, ATL::CComBSTR > {
        string coerce( const ATL::CComBSTR &bstr );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< bool, _variant_t > {
        bool coerce( const _variant_t &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< uint16_t, _variant_t > {
        uint16_t coerce( const _variant_t &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< long, _variant_t > {
        long coerce( const _variant_t &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< float, _variant_t > {
        float coerce( const _variant_t &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< double, _variant_t > {
        double coerce( const _variant_t &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< utf8string, _variant_t > {
        utf8string coerce( const _variant_t &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, _variant_t > {
        string coerce( const _variant_t &v );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< _variant_t, string > {
        _variant_t coerce( const string &str );
    };


    template< typename T >
    struct coercer< nullable< T >, _variant_t > {
        nullable< T > coerce( const _variant_t &f ) {
            if ( f == null )
                return null;
            else
                return coercer< T, _variant_t >().coerce( f );
        }
    };


}


#endif // FOST_COERCE_WIN_HPP

