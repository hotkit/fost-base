/*
    Copyright 2009-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/detail/unicode.hpp>
#include <fost/base64.hpp>
#include <fost/detail/hex.hpp>
#include <fost/parse/parse.hpp>
#include <fost/pointers>

#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


using namespace boost::spirit;
using namespace phoenix;


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


fostlib::utf8_string fostlib::coercer< fostlib::utf8_string, fostlib::string >::coerce(
        const fostlib::string &str ) {
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
fostlib::string fostlib::coercer< fostlib::string, fostlib::utf8_string >::coerce(
        const fostlib::utf8_string &str ) {
    return fostlib::string( str.underlying().c_str(),
        str.underlying().c_str() + str.underlying().length() );
}
fostlib::utf8_string fostlib::coercer<
    fostlib::utf8_string, std::vector< fostlib::utf8 > >::coerce(
        const std::vector< fostlib::utf8 > &str ) {
    return fostlib::utf8_string( &str[0], &str[0] + str.size() );
}
std::vector< fostlib::utf8 > fostlib::coercer<
    std::vector< fostlib::utf8 >, fostlib::utf8_string>::coerce(
        const fostlib::utf8_string &str ) {
    return std::vector< fostlib::utf8 >(
        str.underlying().begin(), str.underlying().end());
}
fostlib::utf8_string fostlib::coercer<
    fostlib::utf8_string, fostlib::const_memory_block >::coerce(
        const fostlib::const_memory_block &block ) {
    if ( block.first && block.second )
        return fostlib::utf8_string(
            reinterpret_cast< const utf8 * >(block.first),
            reinterpret_cast< const utf8 * >(block.second)
        );
    else
        return fostlib::utf8_string();
}


/*
    fostlib::base64_string
*/

namespace {
    static const char *base64_characters =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
    static const char *base64_characters_padded =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/=";

    char base64_encode_6bits(const unsigned char bits) {
        /*
            // This is only ever called from the encoding function below which we know will use the correct number
            // of bits.
            if ( bits > 63 )
                throw fostlib::exceptions::out_of_range< int >( L"Base 64 encoding of 6 bits", 0, 63, bits );
        */
        return base64_characters[bits];
    }

    unsigned char base64_decode_6bits(char v) {
        if ( v >= 'A' && v <= 'Z' )
            return v - 'A';
        else if ( v >= 'a' && v <= 'z' )
            return v - 'a' + 26;
        else if ( v >= '0' && v <= '9' )
            return v - '0' + 52;
        else if ( v == '+' )
            return 62;
        else
            return 63;
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

std::vector< unsigned char > fostlib::detail::base64_decode_3bytes(
    const base64_string &string, base64_string::size_type pos
) {
    const std::string &data = string.underlying().underlying();
    uint32_t bytes = 0;
    int have = -1;
    for ( ; have < 3 && pos < data.length() && data[pos] != '='; ++pos, ++have )
        bytes += base64_decode_6bits(data[pos]) << (6 * (2-have));
    if ( have > 0 ) {
        std::vector< unsigned char > ret(have);
        for ( std::size_t p(0); p != have; ++p )
            ret[p] = static_cast<unsigned char>( ( bytes >> (8 * (2-p)) ) & 0xff );
        return ret;
    } else
        return std::vector< unsigned char >();
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

std::vector< unsigned char > fostlib::coercer<
    std::vector< unsigned char >, fostlib::base64_string
>::coerce( const fostlib::base64_string &v ) {
    const std::string &data = v.underlying().underlying();
    std::vector< unsigned char > ret;
    ret.reserve(1 + data.length() * 3 / 4);
    for ( base64_string::size_type p(0); p < data.length(); p += 4 ) {
        std::vector< unsigned char > bytes( detail::base64_decode_3bytes(v, p) );
        ret.insert(ret.end(), bytes.begin(), bytes.end());
    }
    return ret;
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

fostlib::hex_string fostlib::coercer<
    fostlib::hex_string, std::vector< unsigned char >
>::coerce( const std::vector< unsigned char > &v ) {
    hex_string ret;
    for ( std::vector< unsigned char >::const_iterator i( v.begin() ); i != v.end(); ++i )
        ret += fostlib::coerce< fostlib::hex_string >( *i );
    return ret;
}

std::size_t fostlib::coercer<
    std::size_t, fostlib::hex_string
>::coerce( const fostlib::hex_string &s ) {
    std::size_t ret;
    if ( !parse(
        s.underlying().underlying().c_str(),
        *space_p
            >> uint_parser< std::size_t, 16 >()[ var( ret ) = arg1 ]
            >> *space_p
    ).full )
        throw fostlib::exceptions::parse_error(
            "Whilst parsing a std::size_t", fostlib::coerce< fostlib::string >( s )
        );
    return ret;
}
