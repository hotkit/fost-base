/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COERCE_WIN_HPP
#define FOST_COERCE_WIN_HPP
#pragma once


#ifdef WIN32


#include <fost/coerce.hpp>
#include <fost/null.hpp>


FOST_CORE_DECLSPEC bool operator ==( const VARIANT &l, fostlib::t_null );
inline bool operator ==( fostlib::t_null, const VARIANT &l ) {
    return l == fostlib::null;
}
inline bool operator !=( const VARIANT &l, fostlib::t_null ) {
    return !( l == fostlib::null );
}
inline bool operator !=( fostlib::t_null, const VARIANT &r ) {
    return !( r == fostlib::null );
}


namespace fostlib {


    template<>
    struct FOST_CORE_DECLSPEC coercer< string, HRESULT > {
        string coerce( HRESULT hr );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, BSTR > {
        string coerce( const BSTR &bstr );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< nullable< string >, BSTR > {
        nullable< string > coerce( const BSTR &bstr );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< BSTR, string > {
        BSTR coerce( const string &str );
    };

#ifdef FOST_HAVE_MFC
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, ATL::CComBSTR > {
        string coerce( const ATL::CComBSTR &bstr );
    };
#endif

    template<>
    struct FOST_CORE_DECLSPEC coercer< bool, VARIANT > {
        bool coerce( const VARIANT &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< uint16_t, VARIANT > {
        uint16_t coerce( const VARIANT &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< long, VARIANT > {
        long coerce( const VARIANT &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< float, VARIANT > {
        float coerce( const VARIANT &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< double, VARIANT > {
        double coerce( const VARIANT &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< utf8_string, VARIANT > {
        utf8_string coerce( const VARIANT &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, VARIANT > {
        string coerce( const VARIANT &v );
    };
#ifdef FOST_HAVE_MFC
    template<>
    struct FOST_CORE_DECLSPEC coercer< _variant_t, string > {
        _variant_t coerce( const string &str );
    };
#endif


    template< typename T >
    struct coercer< nullable< T >, VARIANT > {
        nullable< T > coerce( const VARIANT &f ) {
            if ( f == null )
                return null;
            else
                return coercer< T, VARIANT >().coerce( f );
        }
    };


}


#endif // WIN32


#endif // FOST_COERCE_WIN_HPP

