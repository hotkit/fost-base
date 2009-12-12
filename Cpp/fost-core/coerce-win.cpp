/*
    Copyright 2002-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/coerce/win.hpp>
#include <fost/detail/coerce.hpp>


string fostlib::coercer< string, int64_t >::coerce( int64_t t ) {
    wchar_t c[34];  // 34 is the number of powers of 10 to allow for
    _i64tow_s( t, c, 34, 10 );   // 10 is the radix - we want the result in decimal
    return c;
}
string fostlib::coercer< string, uint64_t >::coerce( uint64_t t ) {
    wchar_t c[34];
    _ui64tow_s( t, c, 34, 10 );
    return c;
}
string fostlib::coercer< string, double >::coerce( double f ) {
    char c[ _CVTBUFSIZE ];
    _gcvt_s( reinterpret_cast< char * >( c ), _CVTBUFSIZE, f, 18 );
    return string( c );
}


string fostlib::coercer< string, std::wstring >::coerce( const std::wstring &s ) {
    return string( s.c_str() );
}
std::wstring fostlib::coercer< std::wstring, fostlib::string >::coerce( const fostlib::string &s ) {
    return s.std_str();
}


/*
    COM coercion
*/


bool operator ==( const VARIANT &var, fostlib::t_null ) {
    try {
        switch ( var.vt ) {
        case VT_NULL:
        case VT_EMPTY:
        case VT_ERROR:
            return true;

        case VT_I8:
            return false;

        case VT_DISPATCH:
            return false;

        default:
            return coerce< string >( var ).empty();
        }
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"Within operator ==( VARIANT, null )" << std::endl << L"VARIANT.vt = " << var.vt << std::endl;
        throw;
    }
}


fostlib::string fostlib::coercer< fostlib::string, HRESULT >::coerce( HRESULT hr ) {
    switch ( hr ) {
    case CLASS_E_NOAGGREGATION:
        return L"CLASS_E_NOAGGREGATION";
    case E_NOINTERFACE:
        return L"E_NOINTERFACE";
    case REGDB_E_CLASSNOTREG:
        return L"REGDB_E_CLASSNOTREG";
    default:
        return fostlib::coerce< fostlib::string >( uint64_t( hr ) );
    }
}


string fostlib::coercer< string, BSTR >::coerce( const BSTR &bs ) {
    try {
        if ( bs )
            return string( bs, bs + SysStringLen(bs) );
        else
            return string();
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"Within coercion from _bstr_t to string" << std::endl;
        throw;
    }
}
nullable< string > fostlib::coercer< nullable< string >, BSTR >::coerce( const BSTR &bs ) {
    try {
        if ( bs && SysStringLen(bs) )
            return string( bs, bs + SysStringLen(bs) );
        else
            return string();
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"Within coercion from _bstr_t to string" << std::endl;
        throw;
    }
}


BSTR fostlib::coercer< BSTR, string >::coerce( const string &str ) {
    return SysAllocString( str.c_str() );
}


#ifdef FOST_HAVE_MFC
    string fostlib::coercer< string, ATL::CComBSTR >::coerce( const ATL::CComBSTR &bstr ) {
        return static_cast< const wchar_t * >( bstr );
    }
#endif


#define VCONV( type, vt, memb ) \
    type fostlib::coercer< type, VARIANT >::coerce( const VARIANT &v ) { \
        VARIANT dest; \
        VariantInit(&dest); \
        com_hr( VariantChangeType( &dest, const_cast< VARIANT* >(&v), 0, vt ), L"Converting VARIANT to " L#vt ); \
        type result = dest.memb; \
        VariantClear(&dest); \
        return result; \
    }
VCONV( bool, VT_BOOL, boolVal )
VCONV( uint16_t, VT_UI2, uiVal )
VCONV( long, VT_I4, lVal );
VCONV( float, VT_R4, fltVal );
VCONV( double, VT_R8, dblVal );
VCONV( string, VT_BSTR, bstrVal );
#undef VCONV


utf8_string fostlib::coercer< utf8_string, VARIANT >::coerce( const VARIANT &v ) {
    try {
        return fostlib::coerce< utf8_string >( fostlib::coerce< string >( v ) );
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"Within coercion from VARIANT to utf8_string" << std::endl;
        throw;
    }
}

#ifdef FOST_HAVE_MFC
    _variant_t fostlib::coercer< _variant_t, string >::coerce( const string &s ) {
        try {
            return _variant_t( s.c_str() );
        } catch ( fostlib::exceptions::exception &e ) {
            e.info() << L"Within coercion from fostlib::string to _variant_t" );
            throw;
        }
    }
#endif
