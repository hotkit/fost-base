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


    /// Create RSA private key by base64url_encoded of modulus, public exponent
    /// and private exponent
    CryptoPP::RSA::PrivateKey private_key(
            base64url_encoded n, base64url_encoded e, base64url_encoded d);
    /// Create RSA public key by base64url_encoded of modulus and public exponent
    CryptoPP::RSA::PublicKey
            public_key(base64url_encoded n, base64url_encoded e);


    namespace PKCS1v15_SHA256 {


        /// Return signature for message signed by private key
        base64url_encoded
                sign(base64url_encoded message,
                     CryptoPP::RSA::PrivateKey private_key);
        /// Verify signature of message by public key
        bool validate(
                base64url_encoded message,
                base64url_encoded signature,
                CryptoPP::RSA::PublicKey public_key);


    }


}