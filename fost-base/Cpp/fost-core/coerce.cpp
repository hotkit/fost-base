/*
    Copyright 2007-2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/pointers>
#ifdef _MSC_VER
    #pragma warning ( disable : 4709 ) // comma operator within array index expression
    #pragma warning ( disable : 4244 ) //conversion from 'int' to 'fostlib::utf16', possible loss of data
#endif
#include <fost/parse/parse.hpp>

#include <fost/unicode.hpp>

#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;
using namespace boost::spirit;
using namespace phoenix;


/*
    uint16_t
*/


uint16_t fostlib::coercer< uint16_t, string >::coerce( const string &s ) {
    int ret = 0;
    if ( !parse( s.c_str(), *space_p >> uint_parser< uint16_t >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing a uint16_t", s );
    return ret;
}


/*
    int
*/


int32_t fostlib::coercer< int32_t, string >::coerce( const string &s ) {
    int32_t ret = 0;
    if ( !parse( s.c_str(), *space_p >> int_parser< int32_t >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing an int", s );
    return ret;
}


/*
    int64_t
*/


int64_t fostlib::coercer< int64_t, string >::coerce( const string &s ) {
    int64_t ret = 0;
    if ( !parse( s.c_str(), *space_p >> int_parser< int64_t >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing a int64_t", s );
    return ret;
}


/*
    double
*/


double fostlib::coercer< double, string >::coerce( const string &s ) {
    double ret = 0.0;
    if ( !parse( s.c_str(), *space_p >> real_p[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing a double", s );
    return ret;
}


/*
    string
*/


string fostlib::coercer< string, t_null >::coerce( t_null ) {
    return string();
}
string fostlib::coercer< string, bool >::coerce( bool b ) {
    return b ? L"true" : L"false";
}
string fostlib::coercer< string, std::vector< utf8 > >::coerce( const std::vector< utf8 > &sequence ) {
    utf32 ch; string s;
    for ( std::vector< utf8 >::const_iterator p( sequence.begin() ); p != sequence.end(); p += utf::utf8length( ch ) ) {
        if ( p + 1 == sequence.end() )
            ch = utf::decode( *p );
        else
            ch = utf::decode( *p, *( p + 1 ) );
        s += ch;
    }
    return s;
}
string fostlib::coercer< string, std::vector< wchar_t > >::coerce( const std::vector< wchar_t > &sequence ) {
    utf32 ch; string s;
    for ( std::vector< wchar_t >::const_iterator p( sequence.begin() ); p != sequence.end(); p += utf::utf16length( ch ) ) {
        if ( p + 1 == sequence.end() )
            ch = utf::decode( *p );
        else
            ch = utf::decode( *p, *( p + 1 ) );
        s += ch;
    }
    return s;
}


#ifdef FOST_OS_WINDOWS
    #include "coerce-win.cpp"
#else
    #include "coerce-linux.cpp"
#endif
