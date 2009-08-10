/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/detail/crypto.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


namespace {


    template< unsigned char * (*F)(const unsigned char *,std::size_t,unsigned char *), std::size_t L > inline
    hex_string digest( const string &text ) {
        utf8string toproc( coerce< utf8string >( text ) );
        if ( toproc.length() > std::numeric_limits< unsigned long >::max() )
            throw exceptions::out_of_range< size_t >( L"Message is too long to digest", 0, std::numeric_limits< unsigned long >::max(), toproc.length() );
        boost::array< unsigned char, L > result;
        F( reinterpret_cast< const unsigned char * >( toproc.c_str() ), static_cast< unsigned long>( toproc.length() ), result.data() );
        return coerce< hex_string >( result );
    }


}


string fostlib::md5( const string &text ) {
    return coerce< string >( digest< MD5, MD5_DIGEST_LENGTH >( text ) );
}


string fostlib::sha1( const string &text ) {
    return coerce< string >( digest< SHA1, SHA_DIGEST_LENGTH >( text ) );
}

