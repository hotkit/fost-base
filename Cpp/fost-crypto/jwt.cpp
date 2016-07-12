/*
    Copyright 2007-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/jwt.hpp>

#include <fost/insert>
#include <fost/string>


fostlib::jwt::mint::mint(digester_fn d, const string &k)
: digester(d, k), payload(json::object_t()) {
    insert(header, "typ", "JWT");
    if ( d == sha256 ) {
        insert(header, "alg", "HS256");
    } else {
        throw exceptions::not_implemented(__FUNCTION__,
            "Unknown signing algorithm");
    }
}


fostlib::jwt::mint &fostlib::jwt::mint::subject(const string &s) {
    insert(payload, "sub", s);
    return *this;
}


fostlib::jwt::mint &fostlib::jwt::mint::claim(const string &u, const json &j) {
    insert(payload, u, j);
    return *this;
}


std::string fostlib::jwt::mint::token() {
    std::string str_header, str_payload;
    json::unparse(str_header, header, false);
    json::unparse(str_payload, payload, false);
    auto buffer_header = coerce<base64_string>(
        std::vector<unsigned char>(str_header.begin(), str_header.end()));
    auto buffer_payload = coerce<base64_string>(
        std::vector<unsigned char>(str_payload.begin(), str_payload.end()));
    digester << utf8_string(buffer_header.underlying().underlying())
        << "." << utf8_string(buffer_payload.underlying().underlying());
    return buffer_header.underlying().underlying() + "." +
        buffer_payload.underlying().underlying() + "." +
        coerce<base64_string>(digester.digest()).underlying().underlying();
}

