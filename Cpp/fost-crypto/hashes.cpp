/**
    Copyright 2007-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto.hpp"
#include <fost/string>
#include <fost/crypto.hpp>
#include <fost/exception/out_of_range.hpp>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <crypto++/md5.h>
#include <crypto++/ripemd.h>
#include <crypto++/sha.h>


using namespace fostlib;


namespace {
    template<typename H>
    inline string hash(const f5::u8view &text) {
        boost::array<unsigned char, H::DIGESTSIZE> result;
        H().CalculateDigest(
                result.data(),
                reinterpret_cast<const CryptoPP::byte *>(text.data()),
                text.bytes());
        return coerce<string>(coerce<hex_string>(result));
    }
}


string fostlib::md5(const f5::u8view &text) {
    return hash<CryptoPP::Weak::MD5>(text);
}


string fostlib::sha1(const f5::u8view &text) {
    return hash<CryptoPP::SHA1>(text);
}


string fostlib::sha256(const f5::u8view &text) {
    return hash<CryptoPP::SHA256>(text);
}


string fostlib::ripemd256(const f5::u8view &text) {
    return hash<CryptoPP::RIPEMD256>(text);
}
