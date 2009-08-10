/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/detail/hex.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


/*
    fostlib::ascii_string
*/

void fostlib::ascii_string_tag::do_encode( fostlib::nliteral from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_string_tag::do_encode( fostlib::nliteral from, std::string &into )" );
}
void fostlib::ascii_string_tag::do_encode( const std::string &from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_string_tag::do_encode( const std::string &from, std::string &into )" );
}
void fostlib::ascii_string_tag::check_encoded( const std::string &s ) {
    std::size_t p = 0;
    try {
        for ( std::string::const_iterator c( s.begin() ); c != s.end(); ++c, ++p )
            if ( *c < 1 || *c > 127 )
                throw fostlib::exceptions::out_of_range< int >( L"ASCII characters outside valid range", 1, 127, *c );
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << L"String up until this point: " << fostlib::coerce< fostlib::string >( s.substr( 0, p ) )
            << L"\nChecked " << p << " characters out of " << s.size() << std::endl;
        throw;
    }
}

fostlib::ascii_string fostlib::coercer<
    fostlib::ascii_string, std::vector< fostlib::ascii_string::value_type >
>::coerce( const std::vector< fostlib::ascii_string::value_type > &v ) {
    return ascii_string( std::string( &v[0], v.size() ) );
}


/*
    fostlib::hex_string
*/


namespace {
    static char hex_digits[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
}

void fostlib::hex_string_tag::do_encode( fostlib::nliteral from, ascii_string &into ) {
    throw fostlib::exceptions::not_implemented( "fostlib::hex_string_tag::do_encode( fostlib::nliteral from, ascii_string &into )" );
}

void fostlib::hex_string_tag::do_encode( const ascii_string &from, ascii_string &into ) {
    throw fostlib::exceptions::not_implemented( "fostlib::hex_string_tag::do_encode( const ascii_string &from, ascii_string &into )" );
}

void fostlib::hex_string_tag::check_encoded( const ascii_string &s ) {
    if ( s.underlying().find_first_not_of("0123456789abcdefABCDEF") != std::string::npos )
        throw fostlib::exceptions::parse_error("Non hex character found in hex string", coerce< string >(s));
}


fostlib::hex_string fostlib::coercer< fostlib::hex_string, unsigned char >::coerce( unsigned char c ) {
    hex_string ret;
    ret += hex_digits[ ( c >> 4) & 0xf ];
    ret += hex_digits[ c & 0xf ];
    return ret;
}
