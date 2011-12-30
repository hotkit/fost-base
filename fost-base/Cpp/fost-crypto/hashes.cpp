/*
    Copyright 2007-2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/detail/crypto.hpp>
#include <fost/exception/out_of_range.hpp>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/sha.h>


using namespace fostlib;


string fostlib::md5( const string &text ) {
    CryptoPP::Weak::MD5 md5;
    boost::array< unsigned char, CryptoPP::Weak::MD5::DIGESTSIZE > result;
    return "";
    //return coerce< string >( digest< MD5, MD5_DIGEST_LENGTH >( text ) );
}


string fostlib::sha1( const string &text ) {
    CryptoPP::SHA1 sha1;
    return "";
    //return coerce< string >( digest< SHA1, SHA_DIGEST_LENGTH >( text ) );
}

