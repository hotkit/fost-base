/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/detail/unicode.hpp>
#include <fost/detail/base64.hpp>
#include <fost/detail/hex.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


/*
    fostlib::utf8_string
*/


void fostlib::utf8_string_tag::do_encode( fostlib::nliteral from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::utf8_string_tag::do_encode( fostlib::nliteral from, std::string &into )" );
}
void fostlib::utf8_string_tag::do_encode( const std::string &from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::utf8_string_tag::do_encode( const std::string &from, std::string &into )" );
}
void fostlib::utf8_string_tag::check_encoded( const std::string &s ) {
    // Requesting the Unicode length of the narrow data will check that it is correctly formed as a byproduct
    fostlib::utf::length(s.c_str());
}


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
        e.info() << L"String up until this point: "
            << fostlib::coerce< fostlib::string >( fostlib::coerce< fostlib::ascii_string >( s.substr( 0, p ) ) )
            << L"\nChecked " << p << " characters out of " << s.size() << std::endl;
        throw;
    }
}

fostlib::ascii_string fostlib::coercer<
    fostlib::ascii_string, std::vector< fostlib::ascii_string::value_type >
>::coerce( const std::vector< fostlib::ascii_string::value_type > &v ) {
    return ascii_string( std::string( &v[0], v.size() ) );
}
fostlib::ascii_string fostlib::coercer< fostlib::ascii_string, fostlib::string >::coerce( const fostlib::string &s ) {
    fostlib::utf8string utf8 = fostlib::coerce< fostlib::utf8string >( s );
    return fostlib::ascii_string( utf8.underlying() );
}

fostlib::string fostlib::coercer< fostlib::string, fostlib::ascii_string >::coerce( const fostlib::ascii_string &s ) {
    return string(s.underlying().c_str());
}

std::wstring fostlib::coercer<
    std::wstring, fostlib::ascii_string
>::coerce( const fostlib::ascii_string &s ) {
    std::wstring r; r.resize(s.underlying().length());
    for ( std::size_t p = 0; p < s.underlying().length(); ++p )
        r[p] = s.underlying()[p];
    return r;
}


/*
    fostlib::base64_string
*/

namespace {
    static const char *base64_characters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    ;
    static const char *base64_characters_padded =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="
    ;

    char base64_encode_6bits(const unsigned char bits) {
        /*
            // This is only ever called from the encoding function below which we know will use the correct number
            // of bits.
            if ( bits > 63 )
                throw fostlib::exceptions::out_of_range< int >( L"Base 64 encoding of 6 bits", 0, 63, bits );
        */
        return base64_characters[bits];
    }
}

void fostlib::base64_string_tag::do_encode( fostlib::nliteral from, ascii_string &into ) {
    throw fostlib::exceptions::not_implemented( "fostlib::hex_string_tag::do_encode( fostlib::nliteral from, ascii_string &into )" );
}

void fostlib::base64_string_tag::do_encode( const ascii_string &from, ascii_string &into ) {
    throw fostlib::exceptions::not_implemented( "fostlib::hex_string_tag::do_encode( const ascii_string &from, ascii_string &into )" );
}

void fostlib::base64_string_tag::check_encoded( const ascii_string &s ) {
    if ( s.underlying().find_first_not_of(base64_characters_padded) != std::string::npos )
        throw fostlib::exceptions::parse_error("Non base 64 character found in base 64 string", coerce< string >(s));
}

// Should be passed 3 bytes *or less*. If passed less, then we pad with '='.
fostlib::base64_string fostlib::detail::base64_encode_3bytes( const unsigned char *data, std::size_t length ) {
    if ( length > 3 )
        throw fostlib::exceptions::out_of_range< std::size_t >( L"Base 64 encoding of three bytes", 0, 3, length );
    fostlib::base64_string ret;
    unsigned char bits=0;

    // Okay, what surprised me (and got me) is that we deal with bits in MSB->LSB order. -- dwd

    // Stage one. Extract the high bits of the first byte.
    bits=(data[0]&0xFC)>>2; // 0xFC should be 1111.1100 (Dots seperate nibbles.)
    ret += base64_encode_6bits(bits);

    // Stage two. bottom two bits of the first byte, top four of the second.
    bits=(data[0]&0x03)<<4; // 0x03 should be 0000.0011
    if ( length > 1)
        bits+=(data[1]&0xF0)>>4; // 0xF0 should be 1111.0000 (And if I've got this wrong... :-) )
    ret+=base64_encode_6bits(bits);

    // Stage three. Bottom four of [1], top two of [2].
    if ( length > 1 ) {
        bits=(data[1]&0x0F)<<2; // 0000.1111
        if( length > 2 )
            bits+=(data[2]&0xC0)>>6;
        ret+=base64_encode_6bits(bits); // 1100.000
    } else
        ret+='=';

    // Stage four. Bottom six bits of [2].
    if( length > 2 ) {
        bits=(data[2]&0x3F);
        ret+=base64_encode_6bits(bits);
    } else
        ret+='=';

    return ret;
}

fostlib::base64_string fostlib::coercer< fostlib::base64_string, std::vector< unsigned char > >::coerce(
    const std::vector< unsigned char > &v
) {
    fostlib::base64_string ret;
    std::size_t length = v.size();
    const unsigned char *pos = &v.front();
    for ( ; length > 2; length -= 3, pos += 3 )
        ret += detail::base64_encode_3bytes( pos, 3 );
    return ret + detail::base64_encode_3bytes( pos, length );
}


/*
    fostlib::hex_string
*/


namespace {
    static char hex_digits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
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

fostlib::hex_string fostlib::coercer< fostlib::hex_string, std::vector< unsigned char > >::coerce( const std::vector< unsigned char > &v ) {
    hex_string ret;
    for ( std::vector< unsigned char >::const_iterator i( v.begin() ); i != v.end(); ++i )
        ret += fostlib::coerce< fostlib::hex_string >( *i );
    return ret;
}
