/**
    Copyright 2016-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto.hpp"
#include <fost/ed25519.hpp>
#include <fost/jwt.hpp>

#include <fost/exception/parse_error.hpp>
#include <fost/insert>
#include <fost/log>
#include <fost/string>


namespace {
    template<typename V>
    inline auto base64url(V &&v) {
        auto b64 = fostlib::coerce<fostlib::base64_string>(v);
        fostlib::utf8_string b64u;
        for (const auto c : b64) {
            if (c == '+')
                b64u += '-';
            else if (c == '/')
                b64u += '_';
            else if (c == '=')
                return b64u;
            else
                b64u += c;
        }
        return b64u;
    };
}


/**
    ## `fostlib::jwt::mint`
 */


namespace {
    const fostlib::timestamp c_epoch(1970, 1, 1);
}


fostlib::jwt::mint::mint(alg a, json p)
: algorithm{a}, digester(sha256, ""), m_payload(std::move(p)) {
    insert(header, "typ", "JWT");
    switch (algorithm) {
    case alg::HS256: insert(header, "alg", "HS256"); break;
    case alg::EdDSA: insert(header, "alg", "EdDSA"); break;
    }
}


fostlib::jwt::mint::mint(digester_fn d, const string &k, json p)
: algorithm{alg::HS256}, digester(d, k), m_payload(std::move(p)) {
    insert(header, "typ", "JWT");
    if (d == sha256) {
        insert(header, "alg", "HS256");
    } else {
        throw exceptions::not_implemented(
                __func__, "Unknown signing algorithm");
    }
}


fostlib::jwt::mint::mint(mint &&m)
: digester(std::move(m.digester)),
  header(std::move(m.header)),
  m_payload(std::move(m.m_payload)) {}


fostlib::jwt::mint &fostlib::jwt::mint::subject(const string &s) {
    insert(m_payload, "sub", s);
    return *this;
}


fostlib::timestamp fostlib::jwt::mint::expires(const timediff &tp, bool issued) {
    auto now = timestamp::now();
    const auto exp = now + tp;
    if (issued) insert(m_payload, "iss", (now - c_epoch).total_seconds());
    insert(m_payload, "exp", (exp - c_epoch).total_seconds());
    return exp;
}


fostlib::jwt::mint &fostlib::jwt::mint::claim(f5::u8view u, const json &j) {
    insert(m_payload, u, j);
    return *this;
}


std::string fostlib::jwt::mint::token() {
    std::string str_header, str_payload;
    json::unparse(str_header, header, false);
    json::unparse(str_payload, m_payload, false);
    auto buffer_header = base64url(
            std::vector<unsigned char>(str_header.begin(), str_header.end()));
    auto buffer_payload = base64url(
            std::vector<unsigned char>(str_payload.begin(), str_payload.end()));
    digester << utf8_string(buffer_header.underlying()) << "."
             << utf8_string(buffer_payload.underlying());
    return buffer_header.underlying() + "." + buffer_payload.underlying() + "."
            + base64url(digester.digest()).underlying();
}


std::string fostlib::jwt::mint::token(f5::buffer<const f5::byte> key) {
    std::string str_header, str_payload;
    json::unparse(str_header, header, false);
    json::unparse(str_payload, m_payload, false);
    auto buffer_header = base64url(
            std::vector<unsigned char>(str_header.begin(), str_header.end()));
    auto buffer_payload = base64url(
            std::vector<unsigned char>(str_payload.begin(), str_payload.end()));
    return sign_base64_jwt(buffer_header, buffer_payload, algorithm, key);
}


/**
 * ## JWT signing
 */


std::string fostlib::jwt::sign_base64_jwt(
        f5::u8view header_b64,
        f5::u8view payload_b64,
        alg algorithm,
        f5::buffer<const f5::byte> key) {
    switch (algorithm) {
    case alg::HS256: {
        hmac digester{sha256, key};
        digester << header_b64 << "." << payload_b64;
        return header_b64 + "." + payload_b64 + "."
                + base64url(digester.digest()).underlying();
    }
    case alg::EdDSA: {
        ed25519::keypair const kp{key};
        auto const b64 = header_b64 + "." + payload_b64;
        auto const signature = kp.sign(f5::buffer<const f5::byte>{
                reinterpret_cast<unsigned char const *>(b64.data()),
                b64.size()});
        return b64 + "." + base64url(signature).underlying();
    }
    }
}


/**
    ## fostlib::jwt::token
 */


fostlib::nullable<fostlib::jwt::token> fostlib::jwt::token::load(
        const std::function<std::vector<f5::byte>(json, json)> &lambda,
        f5::u8view t) {
    const auto parts = split(t, ".");
    if (parts.size() != 3u) return fostlib::null;

    try {
        const static json jwt("JWT");
        const static json hs256("HS256");
        const static json eddsa("EdDSA");

        const base64_string b64_header{parts[0].c_str()};
        const auto v64_header = coerce<std::vector<unsigned char>>(b64_header);
        const auto u8_header = coerce<utf8_string>(v64_header);
        const auto str_header = coerce<string>(u8_header);
        const auto header = json::parse(str_header);
        if (header["typ"] != jwt) {
            log::warning(c_fost)("", "JWT type mismatch")("typ", header["typ"]);
        }

        const base64_string b64_payload(parts[1].c_str());
        const auto v64_payload =
                coerce<std::vector<unsigned char>>(b64_payload);
        const auto u8_payload = coerce<utf8_string>(v64_payload);
        const auto str_payload = coerce<string>(u8_payload);
        const auto payload = json::parse(str_payload);

        const base64_string b64_signature(parts[2].c_str());
        const auto v64_signature =
                coerce<std::vector<unsigned char>>(b64_signature);
        if (header["alg"] == hs256) {
            hmac signer(sha256, lambda(header, payload));
            signer << parts[0] << "." << parts[1];
            const auto signature = signer.digest();

            if (not crypto_compare(signature, v64_signature)) {
                log::warning(c_fost)("", "JWT signature mismatch");
                return fostlib::null;
            }
        } else if (header["alg"] == eddsa) {
            throw exceptions::not_implemented(__PRETTY_FUNCTION__);
        } else {
            log::warning(c_fost)("", "JWT algorithm mismatch")(
                    "alg", header["alg"]);
            return fostlib::null;
        }
        if (payload.has_key("exp")) {
            auto exp = c_epoch
                    + fostlib::timediff(fostlib::seconds(
                              fostlib::coerce<int64_t>(payload["exp"])));
            if (exp < fostlib::timestamp::now()) {
                log::warning(c_fost)("", "JWT expired")("expires", exp);
                return fostlib::null;
            }
        }
        return fostlib::jwt::token{header, payload};
    } catch (fostlib::exceptions::parse_error &e) {
        log::warning(c_fost)("", "JWT parse error")("error", e);
        return fostlib::null;
    }
}
