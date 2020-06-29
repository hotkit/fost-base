/**
    Copyright 2018-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <string>

#include <crypto++/config.h>
#include <crypto++/rsa.h>


using base64url_encoded = std::string;


namespace fostlib::rsa {


    namespace PKCS1v15_SHA256 {


        /// Return signature for message signed by private key
        /// n (modulus), e (public exponent) and d (private exponent) are rsa
        /// private key component
        base64url_encoded
                sign(base64url_encoded message,
                     base64url_encoded n,
                     base64url_encoded e,
                     base64url_encoded d);

        /// Verify signature of message by public key
        /// n (modulus) and e (public exponent) are rsa private key component
        bool validate(
                base64url_encoded message,
                base64url_encoded signature,
                base64url_encoded n,
                base64url_encoded e);


    }


}
