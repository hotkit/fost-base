/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/crypto.hpp>
#include <openssl/hmac.h>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


namespace {


    utf8string base64_encode( const unsigned char *pos, std::size_t length );


}


string fostlib::sha1_hmac( const string &key, const string &data ) {
    utf8string key_utf8( coerce< utf8string >( key ) ), data_utf8( coerce< utf8string >( data ) );
    if ( key_utf8.length() > std::numeric_limits< int >::max() )
        throw exceptions::out_of_range< uint64_t >( L"Key length is too long", 0, std::numeric_limits< int >::max(), key_utf8.length() );
    if ( data_utf8.length() > std::numeric_limits< int >::max() )
        throw exceptions::out_of_range< uint64_t >( L"Message data is too long", 0, std::numeric_limits< int >::max(), data_utf8.length() );

    unsigned char signature[ EVP_MAX_MD_SIZE ] = {0};
    unsigned int signature_length = 0;

    HMAC( EVP_sha1(), key_utf8.data(), static_cast< int >( key_utf8.length() ), reinterpret_cast< const unsigned char * >( data_utf8.data() ), static_cast< int >( data_utf8.length() ), signature, &signature_length );
    return coerce< string >( base64_encode( signature, signature_length ) );
}


namespace {


    // For the following, bits MUST be less than 64. In other words, 6 bits only.
    char base64_encode_6bits( const unsigned short int bits ) {
        if( bits > 63 ) {
            throw exceptions::out_of_range< int >( L"Base 64 encoding of 6 bits", 0, 63, bits );
        }
        if ( bits < 26 ) {
            return char( ( 'A' + bits ) );
        } else if ( bits < 52 ) {
            return char( 'a' + ( bits - 26 ) );
        } else if ( bits < 62 ) {
            return char( '0' + ( bits - 52 ) );
        } else if ( bits == 62 ) {
            return '+';
        } else { // if(bits==63) - which it must do.
            return '/';
        }
    }


    // Should be passed 3 bytes *or less*. If passed less, then we pad with '='.
    utf8string base64_encode_3bytes( const unsigned char *data, std::size_t length ) {
        if ( length > 3 )
            throw exceptions::out_of_range< std::size_t >( L"Base 64 encoding of three bytes", 0, 3, length );
        utf8string ret;
        unsigned short int bits=0;

        // Okay, what surprised me (and got me) is that we deal with bits in MSB->LSB order.

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


    utf8string base64_encode( const unsigned char *pos, std::size_t length ) {
        utf8string ret;
        for ( ; length > 2; length -= 3, pos += 3 )
            ret += base64_encode_3bytes( pos, 3 );
        return ret + base64_encode_3bytes( pos, length );
    }


}
