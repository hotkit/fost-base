/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/jwt.hpp>

#include <fost/exception/parse_error.hpp>
#include <fost/insert>
#include <fost/log>
#include <fost/string>


/*
    fostlib::jwt::mint
 */


namespace {
    const fostlib::timestamp c_epoch(1970, 1, 1);
}


fostlib::jwt::mint::mint(digester_fn d, const string &k)
: digester(d, k), m_payload(json::object_t()) {
    insert(header, "typ", "JWT");
    if ( d == sha256 ) {
        insert(header, "alg", "HS256");
    } else {
        throw exceptions::not_implemented(__FUNCTION__,
            "Unknown signing algorithm");
    }
}


fostlib::jwt::mint::mint(mint &&m)
: digester(std::move(m.digester)),
    header(std::move(m.header)),
    m_payload(std::move(m.m_payload))
{
}


fostlib::jwt::mint &fostlib::jwt::mint::subject(const string &s) {
    insert(m_payload, "sub", s);
    return *this;
}


fostlib::timestamp fostlib::jwt::mint::expires(const timediff &tp, bool issued) {
    auto now = timestamp::now();
    const auto exp = now + tp;
    if ( issued )
        insert(m_payload, "iss", (now - c_epoch).total_seconds());
    insert(m_payload, "exp", (exp - c_epoch).total_seconds());
    return exp;
}


fostlib::jwt::mint &fostlib::jwt::mint::claim(const string &u, const json &j) {
    insert(m_payload, u, j);
    return *this;
}


std::string fostlib::jwt::mint::token() {
    std::string str_header, str_payload;
    json::unparse(str_header, header, false);
    json::unparse(str_payload, m_payload, false);
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


/*
    fostlib::jwt::token
 */


fostlib::nullable<fostlib::jwt::token> fostlib::jwt::token::load(const string &secret, const string &t) {
    const auto parts = split(t, ".");
    if ( parts.size() != 3u ) return fostlib::null;

    try {
        const static json jwt("JWT");
        const static json hs256("HS256");

        const base64_string b64_header{parts[0].c_str()};
        const auto v64_header = coerce<std::vector<unsigned char>>(b64_header);
        const auto u8_header = coerce<utf8_string>(v64_header);
        const auto str_header = coerce<string>(u8_header);
        const auto header = json::parse(str_header);
        if ( header["typ"] != jwt || header["alg"] != hs256 ) {
            log::warning(c_fost)
                ("", "JWT type or algorithm mismatch")
                ("typ", header["typ"])
                ("alg", header["alg"]);
            return fostlib::null;
        }

        const base64_string b64_payload(parts[1].c_str());
        const auto v64_payload = coerce<std::vector<unsigned char>>(b64_payload);
        const auto u8_payload = coerce<utf8_string>(v64_payload);
        const auto str_payload = coerce<string>(u8_payload);
        const auto payload = json::parse(str_payload);

        const base64_string b64_signature(parts[2].c_str());
        const auto v64_signature = coerce<std::vector<unsigned char>>(b64_signature);
        hmac signer(sha256, secret);
        signer << parts[0] << "." << parts[1];
        const auto signature = signer.digest();

        if ( crypto_compare(signature, v64_signature) ) {
            if ( payload.has_key("exp") ) {
                auto exp = c_epoch + fostlib::timediff(
                    fostlib::seconds(fostlib::coerce<int64_t>(payload["exp"])));
                if ( exp < fostlib::timestamp::now() ) {
                    log::warning(c_fost)
                        ("", "JWT expired")
                        ("expires", exp);
                    return fostlib::null;
                }
            }
            return fostlib::jwt::token{header, payload};
        } else {
            log::warning(c_fost)("", "JWT signature mismatch");
            return fostlib::null;
        }
    } catch ( fostlib::exceptions::parse_error &e ) {
        log::warning(c_fost)
            ("", "JWT parse error")
            ("error", e);
        return fostlib::null;
    }
}

