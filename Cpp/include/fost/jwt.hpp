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

            /// Return the token
            std::string token();
        };


    }


}
