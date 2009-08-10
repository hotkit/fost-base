/*
    Copyright 2002-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <atlbase.h>
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


bool operator ==( const _variant_t &var, fostlib::t_null ) {
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
        e.info() << L"Within operator ==( _variant_t, null )" << std::endl << L"VARIANT.vt = " << var.vt << std::endl;
        throw;
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within operator ==( _variant_t, null )" );
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


string fostlib::coercer< string, _bstr_t >::coerce( const _bstr_t &bstr ) {
    try {
        return string( static_cast< const wchar_t * >( bstr ) );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _bstr_t to string" );
    }
}
nullable< string > fostlib::coercer< nullable< string >, _bstr_t >::coerce( const _bstr_t &bs ) {
    try {
        // We might need some special handling here for _bstr_t created from NULL
        string ret = fostlib::coerce< string >( bs );
        if ( ret.empty() )
            return null;
        else
            return ret;
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _bstr_t to nullable< string >" );
    }
}


_bstr_t fostlib::coercer< _bstr_t, string >::coerce( const string &str ) {
    try {
        return _bstr_t( str.c_str() );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from string to _bstr_t" );
    }
}


string fostlib::coercer< string, ATL::CComBSTR >::coerce( const ATL::CComBSTR &bstr ) {
    return static_cast< const wchar_t * >( bstr );
}


bool fostlib::coercer< bool, _variant_t >::coerce( const _variant_t &v ) {
    try {
        return static_cast< bool >( v );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to bool" );
    }
}


uint16_t fostlib::coercer< uint16_t, _variant_t >::coerce( const _variant_t &v ) {
    try {
        return static_cast< uint16_t >( v );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to uint16_t" );
    }
}


long fostlib::coercer< long, _variant_t >::coerce( const _variant_t &v ) {
    try {
        if ( v == null )
            throw fostlib::exceptions::null( L"COM variant was empty" );
        return static_cast< long >( v );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to long" );
    }
}


#pragma warning( push )
#pragma warning( disable : 4702 )
float fostlib::coercer< float, _variant_t >::coerce( const _variant_t &v ) {
    try {
        return float( v );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to float" );
    }
}


double fostlib::coercer< double, _variant_t >::coerce( const _variant_t &v ) {
    try {
        return static_cast< double >( v );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to double" );
    }
}
#pragma warning( pop )


utf8string fostlib::coercer< utf8string, _variant_t >::coerce( const _variant_t &v ) {
    try {
        return fostlib::coerce< utf8string >( fostlib::coerce< string >( v ) );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to utf8string" );
    }
}


string fostlib::coercer< string, _variant_t >::coerce( const _variant_t &v ) {
    try {
        return string( static_cast< const wchar_t * >( static_cast< const _bstr_t >( v ) ) );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from _variant_t to string" );
    }
}
_variant_t fostlib::coercer< _variant_t, string >::coerce( const string &s ) {
    try {
        return _variant_t( s.c_str() );
    } catch ( _com_error &c ) {
        throw fostlib::exceptions::com_error( c, L"Within coercion from string to _variant_t" );
    }
}

