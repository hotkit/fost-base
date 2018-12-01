/**
    Copyright 2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
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

        fostlib::nacl::crypto_hash(
                d, reinterpret_cast<const uint8_t *>(sk.data()), sk.size());
        d[0] &= 248;
        d[31] &= 127;
        d[31] |= 64;
        fostlib::nacl::scalarbase(p, d);

        fostlib::ed25519::secret pk;
        fostlib::nacl::pack(reinterpret_cast<uint8_t *>(pk.data()), p);
        return pk;
    }


}


fostlib::ed25519::keypair::keypair() : keypair(crypto_bytes<32>()) {}


fostlib::ed25519::keypair::keypair(secret sk) {
    std::copy(sk.begin(), sk.end(), privkey.begin());
    auto pk = signing_pubkey(priv());
    std::copy(pk.begin(), pk.end(), privkey.begin() + 32);
}


fostlib::ed25519::keypair::keypair(f5::buffer<const f5::byte> sk) {
    std::copy(sk.begin(), sk.end(), privkey.begin());
}


std::array<f5::byte, 64> fostlib::ed25519::keypair::sign(
        f5::buffer<const f5::byte> message) const {
    std::vector<f5::byte> signature(message.size() + 64);
    uint64_t siglen{signature.size()};
    fostlib::nacl::crypto_sign(
            signature.data(), &siglen, message.data(), message.size(),
            privkey.data());
    if (siglen < 64)
        throw exceptions::not_implemented(
                __PRETTY_FUNCTION__,
                "Unexpected ed25519 signature length - too short", siglen);
    std::array<f5::byte, 64> ret;
    std::copy(signature.begin(), signature.begin() + 64, ret.begin());
    return ret;
}


bool fostlib::ed25519::verify(
        secret pub,
        f5::buffer<const f5::byte> msg,
        f5::buffer<const f5::byte> sig) {
    /// Because NaCl has a stupid API we have to build it's message format out
    /// of the parts we have before we can do anything else
    std::vector<f5::byte> msgsig(msg.size() + sig.size());
    std::copy(sig.begin(), sig.end(), msgsig.begin());
    std::copy(msg.begin(), msg.end(), msgsig.begin() + sig.size());
    /// We also need a buffer for NaCl to give us the message back in (!) so
    /// that we can ignore it because we actually got handed the message as a
    /// parameter. Note that the outmsg allocation is larger than might be
    /// expected as the NaCl documentation tells us we must allocate enough for
    /// the message and the secret (for some reason).
    std::vector<f5::byte> outmsg(msgsig.size());
    uint64_t outmsg_len{};
    if (fostlib::nacl::crypto_sign_open(
                outmsg.data(), &outmsg_len, msgsig.data(), msgsig.size(),
                pub.data())
        == 0) {
        if (outmsg_len != msg.size()) {
            /// If this check is ever false then probably we're in  a lot of
            /// trouble, especially if the output buffer is  now longer than the
            /// one we allocated
            throw exceptions::not_implemented(
                    __PRETTY_FUNCTION__,
                    "Something truly awful happened and the message somehow "
                    "grew");
        }
        return true;
    } else {
        return false;
    }
}
