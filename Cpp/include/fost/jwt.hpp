/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <fost/crypto.hpp>


namespace fostlib {


    namespace jwt {


        /// The digest algorithms that are supported
        enum digest {hs256};

        /// The encryption algorithms that are supported
        enum encryption {};


        /// Create a JWT
        class mint {
            hmac digester;
            json header, payload;
        public:
            /// Set up for creating a signed JWT
            mint(digester_fn d, const string &key);

            /// Set the subject claim
            mint &subject(const string &);

            /// Set a claim. If the claim name is not listed at
            /// http://www.iana.org/assignments/jwt/jwt.xhtml then
            /// it should be a URL. See RFC7519
            mint &claim(const string &url, const json &value);

            /// Return the token
            std::string token();
        };


        /// Check a JWT
        struct token {
            /// Load the token and return it if verified
            static nullable<token> load(const string &secret, const string &jwt);
            /// The token header and payload
            const json header, payload;
        };


    }


}
