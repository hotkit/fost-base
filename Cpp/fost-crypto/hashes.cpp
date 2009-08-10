/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/crypto.hpp>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


namespace {


    string hex( const unsigned char *p, size_t l ) {
        static wchar_t digits[ 16 ] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        string ret;
        for ( size_t i( 0 ); i < l; ++i ) {
            ret += digits[ ( p[i] >> 4) & 0xF ];
            ret += digits[ p[i] & 0xF ];
        }
        return ret;
    }


    template< unsigned char * (*F)(const unsigned char *,std::size_t,unsigned char *), std::size_t L > inline
    string digest( const string &text ) {
        utf8string toproc( coerce< utf8string >( text ) );
        if ( toproc.length() > std::numeric_limits< unsigned long >::max() )
            throw exceptions::out_of_range< size_t >( L"Message is too long to digest", 0, std::numeric_limits< unsigned long >::max(), toproc.length() );
        boost::scoped_array< unsigned char > result( new unsigned char[ L ] );
        F( reinterpret_cast< const unsigned char * >( toproc.c_str() ), static_cast< unsigned long>( toproc.length() ), result.get() );
        return hex( result.get(), L );
    }


}


string fostlib::md5( const string &text ) {
    return digest< MD5, MD5_DIGEST_LENGTH >( text );
}


string fostlib::sha1( const string &text ) {
    return digest< SHA1, SHA_DIGEST_LENGTH >( text );
}

