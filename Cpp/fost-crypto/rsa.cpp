/**
    Copyright 2018-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <string>

#include <crypto++/base64.h>
#include <crypto++/config.h>
#include <crypto++/osrng.h>
#include <crypto++/rsa.h>

#include <fost/crypto>
#include <fost/rsa.hpp>
#include <fost/test>


CryptoPP::RSA::PrivateKey fostlib::rsa::private_key(base64url_encoded n, base64url_encoded e, base64url_encoded d) {
    CryptoPP::StringSource decoded_n(
            n, true, new CryptoPP::Base64URLDecoder);
    CryptoPP::StringSource decoded_e(
            e, true, new CryptoPP::Base64URLDecoder);
    CryptoPP::StringSource decoded_d(
            d, true, new CryptoPP::Base64URLDecoder);

    CryptoPP::Integer int_n(decoded_n, decoded_n.MaxRetrievable());
    CryptoPP::Integer int_e(decoded_e, decoded_e.MaxRetrievable());
    CryptoPP::Integer int_d(decoded_d, decoded_d.MaxRetrievable());

    CryptoPP::RSA::PrivateKey privateKey;
    privateKey.Initialize(int_n, int_e, int_d);
    return privateKey;
}


CryptoPP::RSA::PublicKey fostlib::rsa::public_key(base64url_encoded n, base64url_encoded e) {
    CryptoPP::StringSource decoded_n(
            n, true, new CryptoPP::Base64URLDecoder);
    CryptoPP::StringSource decoded_e(
            e, true, new CryptoPP::Base64URLDecoder);

    CryptoPP::Integer int_n(decoded_n, decoded_n.MaxRetrievable());
    CryptoPP::Integer int_e(decoded_e, decoded_e.MaxRetrievable());

    CryptoPP::RSA::PublicKey public_key;
    public_key.Initialize(int_n, int_e);
    return public_key;
}


base64url_encoded fostlib::rsa::PKCS1v15_SHA256::sign(base64url_encoded message, CryptoPP::RSA::PrivateKey private_key) {
    CryptoPP::AutoSeededRandomPool prng;
    CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA256>::Signer signer(
            private_key);    
    
    base64url_encoded signature;
    CryptoPP::StringSource ss(
            message, true,
            new CryptoPP::SignerFilter(
                    prng, signer,
                    new CryptoPP::Base64URLEncoder(
                            new CryptoPP::StringSink(signature))));
    return signature;
}


bool validate(base64url_encoded message, base64url_encoded signature, CryptoPP::RSA::PublicKey public_key) {
    CryptoPP::RSASS<CryptoPP::PKCS1v15, CryptoPP::SHA256>::Verifier verifier(
            public_key);    

    base64url_encoded decoded_sig;
    CryptoPP::StringSource ss(
            signature, true,
            new CryptoPP::Base64URLDecoder(
                    new CryptoPP::StringSink(decoded_sig))
    );  

    return verifier.VerifyMessage(
            (const CryptoPP::byte *)message.c_str(), message.length(),
            (const CryptoPP::byte *)decoded_sig.c_str(), decoded_sig.length());
}

