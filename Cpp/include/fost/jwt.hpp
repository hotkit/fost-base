/**
    Copyright 2016-2018, Felspar Co Ltd. http://support.felspar.com/

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <fost/datetime>
#include <fost/crypto.hpp>


namespace fostlib {


    namespace jwt {


        /// The digest algorithms that are supported
        enum digest { hs256 };

        /// The encryption algorithms that are supported
        enum encryption {};


        /// Create a JWT
        class mint {
            hmac digester;
            json header, m_payload;

          public:
            /// Set up for creating a signed JWT
            mint(digester_fn d,
                 const string &key,
                 json payload = json::object_t{});
            /// Make movable
            mint(mint &&);

            /// Set the subject claim
            mint &subject(const string &);

            /// Set the token to expire after this amount of time
            timestamp expires(const timediff &, bool issued = true);

            /// Set a claim. If the claim name is not listed at
            /// http://www.iana.org/assignments/jwt/jwt.xhtml then
            /// it should be a URL. See RFC7519
            mint &claim(f5::u8view url, const json &value);

            /// Return the token
            std::string token();

            /// Return the current payload
            const json &payload() const { return m_payload; }
        };


        /// Check a JWT
        struct token {
            /// Load the token with a secret returned by the lambda
            static nullable<token>
                    load(const std::function<string(json, json)> &lambda,
                         f5::u8view jwt);
            /// Load the token and return it if verified
            static nullable<token> load(string secret, f5::u8view jwt) {
                return load(
                        [secret = std::move(secret)](json, json) {
                            return secret;
                        },
                        jwt);
            }
            /// The token header and payload
            const json header, payload;
        };


    }


}
