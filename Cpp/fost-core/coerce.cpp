/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#ifdef _MSC_VER
    #pragma warning ( disable : 4709 ) // comma operator within array index expression
    #pragma warning ( disable : 4244 ) //conversion from 'int' to 'fostlib::utf16', possible loss of data
#endif
#include <boost/spirit/core.hpp>
#include <boost/spirit/attribute.hpp>
#include <boost/spirit/utility/confix.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/functions.hpp>
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
    int ret;
    if ( !parse( s.c_str(), *space_p >> uint_parser< uint16_t >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing a uint16_t", s );
    return ret;
}


/*
    int
*/


int fostlib::coercer< int, int64_t >::coerce( int64_t i ) {
    if ( i > std::numeric_limits< int >::max() || i < std::numeric_limits< int >::min() )
        throw fostlib::exceptions::out_of_range< int64_t >( std::numeric_limits< int >::min(), std::numeric_limits< int >::max(), i );
    return int( i );
}
int fostlib::coercer< int, string >::coerce( const string &s ) {
    int ret;
    if ( !parse( s.c_str(), *space_p >> int_parser< int >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing an int", s );
    return ret;
}


/*
    long
*/


#ifdef FOST_USE_LONG
long fostlib::coercer< long, int64_t >::coerce( int64_t i ) {
    if ( i > std::numeric_limits< long >::max() || i < std::numeric_limits< long >::min() )
        throw fostlib::exceptions::out_of_range< int64_t >( std::numeric_limits< long >::min(), std::numeric_limits< long >::max(), i );
    return long( i );
}
#endif // FOST_USE_LONG


/*
    int64_t
*/


int64_t fostlib::coercer< int64_t, string >::coerce( const string &s ) {
    int64_t ret;
    if ( !parse( s.c_str(), *space_p >> int_parser< int64_t >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing a int64_t", s );
    return ret;
}


/*
    double
*/


double fostlib::coercer< double, string >::coerce( const string &s ) {
    double ret;
    if ( !parse( s.c_str(), *space_p >> real_p[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing a double", s );
    return ret;
}


/*
    utf8string
*/

utf8string fostlib::coercer< utf8string, string >::coerce( const string &str ) {
    utf8string ret;
    ret.reserve( str.native_length() );
    utf8 buffer[ utf::utf32_utf8_max_length ];
    for ( string::const_iterator it( str.begin() ); it != str.end(); ++it ) {
        utf32 c( *it );
        try {
            ret.append( buffer, buffer + utf::encode( c, buffer, buffer + utf::utf32_utf8_max_length ) );
        } catch ( exceptions::exception &e ) {
            e.info() << L"Character: " << fostlib::coerce< string >( int( c ) ) << std::endl;
            throw;
        }
    }

    return ret;
}
string fostlib::coercer< string, utf8string >::coerce( const utf8string &str ) {
    return string( str.c_str(), str.c_str() + str.length() );
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
string fostlib::coercer< string, int >::coerce( int i ) {
    return fostlib::coerce< string >( int64_t( i ) );
}
string fostlib::coercer< string, unsigned int >::coerce( unsigned int i ) {
    return fostlib::coerce< string >( uint64_t( i ) );
}
#ifdef FOST_USE_LONG
string fostlib::coercer< string, unsigned long >::coerce( unsigned long i ) {
    return fostlib::coerce< string >( uint64_t( i ) );
}
#endif // FOST_USE_LONG


#ifdef WIN32
    #include "coerce-win.cpp"
#else
    #include "coerce-linux.cpp"
#endif
