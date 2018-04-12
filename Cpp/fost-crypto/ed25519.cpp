/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/crypto.hpp>
#include <fost/ed25519.hpp>

#include "naclite.h"


namespace {


    auto signing_pubkey(f5::buffer<const f5::byte> sk) {
        // Derived from crypto_sign_keypair in Crypto++ tweetnacl.cpp
        uint8_t d[64];
        fostlib::nacl::gf p[4];

        fostlib::nacl::crypto_hash(d, reinterpret_cast<const uint8_t*>(sk.data()), sk.size());
        d[0] &= 248;
        d[31] &= 127;
        d[31] |= 64;
        fostlib::nacl::scalarbase(p, d);

        fostlib::ed25519::secret pk;
        fostlib::nacl::pack(reinterpret_cast<uint8_t*>(pk.data()), p);
        return pk;
    }


}


fostlib::ed25519::keypair::keypair()
: keypair(crypto_bytes<32>()) {
}


fostlib::ed25519::keypair::keypair(secret sk) {
    std::copy(sk.begin(), sk.end(), privkey.begin());
    auto pk = signing_pubkey(priv());
    std::copy(pk.begin(), pk.end(), privkey.begin() + 32);
}


std::array<f5::byte, 64> fostlib::ed25519::keypair::sign(
    f5::buffer<const f5::byte> data
) const {
    std::vector<f5::byte> signature(data.size() + 64);
    uint64_t siglen{signature.size()};
    fostlib::nacl::crypto_sign(
        signature.data(), &siglen,
        data.data(), data.size(),
        privkey.data());
    if ( siglen < 64 )
        throw fostlib::exceptions::not_implemented(__func__,
            "Unexpected ed25519 signature length - too short", siglen);
    std::array<f5::byte, 64> ret;
    std::copy(signature.begin(), signature.begin() + 64, ret.begin());
    return ret;
}

