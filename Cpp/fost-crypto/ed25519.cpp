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


    auto signing_pubkey(const fostlib::ed25519::secret sk) {
        // Derived from crypto_sign_keypair in Crypt++ tweetnacl.cpp
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
: priv{crypto_bytes<32>()}, pub{signing_pubkey(priv)} {
}


fostlib::ed25519::keypair::keypair(secret s)
: priv(s), pub{signing_pubkey(priv)} {
}

