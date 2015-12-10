/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/password.hpp>


std::array<unsigned char, 64> fostlib::pbkdf2_hmac_sha256(
    const fostlib::utf8_string &password, fostlib::array_view<unsigned char> salt, std::size_t rounds
) {
    throw fostlib::exceptions::not_implemented(__FUNCTION__);
}

