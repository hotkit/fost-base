/**
    Copyright 2016-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto.hpp"
#include <fost/crypto.hpp>
#include <fost/nonce.hpp>


namespace {
    template<std::size_t Size>
    fostlib::base64_string nonce() {
        const auto base64url = [](auto &&v) {
            fostlib::utf8_string b64u;
            for (const auto c : v) {
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
        const auto bytes = fostlib::crypto_bytes<Size>();
        const auto b64 = fostlib::coerce<fostlib::base64_string>(
                std::vector<unsigned char>(bytes.begin(), bytes.end()));
        return base64url(b64).underlying().c_str();
    }
}


fostlib::base64_string fostlib::nonce8b64u() { return nonce<8>(); }
fostlib::base64_string fostlib::nonce24b64u() { return nonce<24>(); }
fostlib::base64_string fostlib::nonce32b64u() { return nonce<32>(); }
