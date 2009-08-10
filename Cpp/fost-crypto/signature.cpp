/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/crypto.hpp>
#include <openssl/hmac.h>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


namespace {


    utf8string base64_encode( const unsigned char *pos, std::size_t length );


}


string fostlib::sha1_hmac( const string &key, const string &data ) {
    BOOST_STATIC_ASSERT(sizeof(std::size_t)>=sizeof(int));
    utf8string key_utf8( coerce< utf8string >( key ) ), data_utf8( coerce< utf8string >( data ) );
    if ( key_utf8.length() > std::size_t(std::numeric_limits< int >::max()) )
        throw exceptions::out_of_range< uint64_t >( L"Key length is too long", 0, std::numeric_limits< int >::max(), key_utf8.length() );
    if ( data_utf8.length() > std::size_t(std::numeric_limits< int >::max()) )
        throw exceptions::out_of_range< uint64_t >( L"Message data is too long", 0, std::numeric_limits< int >::max(), data_utf8.length() );

    unsigned char signature[EVP_MAX_MD_SIZE] = {0};
    unsigned int signature_length = 0;

    HMAC(
        EVP_sha1(), key_utf8.data(), static_cast< int >( key_utf8.length() ),
        reinterpret_cast< const unsigned char * >( data_utf8.data() ), static_cast< int >( data_utf8.length() ), signature,
        &signature_length
    );
    return coerce< string >( coerce< base64_string >( std::vector< unsigned char >( signature, signature + signature_length ) ) );
}

