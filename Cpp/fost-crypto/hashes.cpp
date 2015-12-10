/*
    Copyright 2007-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/crypto.hpp>
#include <fost/exception/out_of_range.hpp>

#ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/sha.h>

using namespace fostlib;


const module fostlib::c_fost_crypto(c_fost, "crypto");


namespace {
    template< typename H >
    string hash( const string &text ) {
        utf8_string toproc( coerce< utf8_string >( text ) );
        boost::array< unsigned char, H::DIGESTSIZE > result;
        H().CalculateDigest(
            result.data(),
            reinterpret_cast<const unsigned char *>(toproc.underlying().c_str()),
            toproc.underlying().length());
        return coerce<string>(coerce< hex_string >( result ));
    }
}


string fostlib::md5(const string &text) {
    return hash<CryptoPP::Weak::MD5>(text);
}


string fostlib::sha1(const string &text) {
    return hash<CryptoPP::SHA1>(text);
}


string fostlib::sha256(const string &text) {
    return hash<CryptoPP::SHA256>(text);
}
