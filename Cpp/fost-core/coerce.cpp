/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
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
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/phoenix1.hpp>

#include <fost/detail/unicode.hpp>

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


int32_t fostlib::coercer< int32_t, string >::coerce( const string &s ) {
    int32_t ret;
    if ( !parse( s.c_str(), *space_p >> int_parser< int32_t >()[ var( ret ) = arg1 ] >> *space_p ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing an int", s );
    return ret;
}


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
    uint64_t
*/


uint64_t fostlib::coercer< uint64_t, int64_t >::coerce( int64_t i ) {
    if ( i < 0 )
        throw fostlib::exceptions::out_of_range< int64_t >( std::numeric_limits< uint64_t >::min(), std::numeric_limits< uint64_t >::max(), i );
    return uint64_t( i );
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
    utf8_string
*/

utf8_string fostlib::coercer< utf8_string, string >::coerce( const string &str ) {
    std::string ret;
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

    return utf8_string(ret);
}
string fostlib::coercer< string, utf8_string >::coerce( const utf8_string &str ) {
    return string( str.underlying().c_str(), str.underlying().c_str() + str.underlying().length() );
}
utf8_string fostlib::coercer< utf8_string, std::vector< utf8 > >::coerce( const std::vector< utf8 > &str ) {
    return utf8_string( &str[0], &str[0] + str.size() );
}
utf8_string fostlib::coercer< utf8_string, const_memory_block >::coerce( const const_memory_block &block ) {
    if ( block.first && block.second )
        return utf8_string(
            reinterpret_cast< const utf8 * >(block.first),
            reinterpret_cast< const utf8 * >(block.second)
        );
    else
        return utf8_string();
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
