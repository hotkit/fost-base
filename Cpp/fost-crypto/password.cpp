/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/password.hpp>

#ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wunused-function"
#endif
#include <crypto++/sha.h>
#include <crypto++/pwdbased.h>


namespace {
    template< typename R>
    void derive(
        R &result,
        const fostlib::utf8_string &password,
        const fostlib::array_view<unsigned char> &salt,
        std::size_t rounds
    ) {
        CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA256> pbkdf2;
        pbkdf2.DeriveKey(result.data(), result.size(), 0 /* purpose isn't used */,
            reinterpret_cast<const unsigned char *>(password.underlying().c_str()),
                password.underlying().length(),
            salt.data(), salt.size(),
            rounds, 0.0 /* Don't send time */);
    }
}


std::array<unsigned char, 64> fostlib::pbkdf2_hmac_sha256(
    const fostlib::utf8_string &password,
    fostlib::array_view<unsigned char> salt,
    std::size_t rounds
) {
    std::array<unsigned char, 64> result;
    derive(result, password, salt, rounds);
    return result;
}

std::vector<unsigned char> fostlib::pbkdf2_hmac_sha256(
    const utf8_string &password,
    array_view<unsigned char> salt,
    std::size_t rounds, std::size_t length
) {
    std::vector<unsigned char> result(length);
    derive(result, password, salt, rounds);
    return result;
}
