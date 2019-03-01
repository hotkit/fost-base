/**
    Copyright 2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <f5/memory.hpp>
#include <fost/array>

#include <string>


namespace fostlib::ed25519 {


    /// A public or private key or other secret
    using secret = std::array<f5::byte, 32>;


    /// A keypair for ed25519
    class keypair {
        /// The way the library we use thinks about the keys is that
        /// there is a public key (32 bytes) and a secret key (64 bytes).
        /// The secret is of course 32 bytes, but it's got the public key
        /// appended to it (!!). Lots of the library implementations
        /// rely on thiis :(
        ///
        /// We'll just store the two parts in a single 64 byte array
        /// and call it done. The `priv` and `pub` accessors return
        /// the correct parts.
        std::array<f5::byte, 64> privkey;

      public:
        /// Create a new keypair
        explicit keypair();
        /// Create a keypair from a secret
        explicit keypair(secret);
        /// Create a keypair from a memory buffer
        keypair(f5::buffer<const f5::byte>);

        /// Return the secret and private parts
        secret priv() const {
            secret s;
            std::copy(privkey.data(), privkey.data() + 32, s.begin());
            return s;
        }
        secret pub() const {
            secret s;
            std::copy(privkey.data() + 32, privkey.data() + 64, s.begin());
            return s;
        }

        /// Return a signature for the presented data
        std::array<f5::byte, 64> sign(f5::buffer<const f5::byte> data) const;

        /// Allow conversion of the keypair to a memory buffer
        operator f5::buffer<const f5::byte>() const { return privkey; }
    };


    /// Verify a signed message has been signed with the provided public key
    bool
            verify(f5::buffer<const f5::byte> pub,
                   f5::buffer<const f5::byte> msg,
                   f5::buffer<const f5::byte> sig);

    inline bool
            verify(f5::buffer<const f5::byte> pub,
                   const std::string &msg,
                   f5::buffer<const f5::byte> sig) {
        return verify(
                pub,
                f5::buffer<const f5::byte>{
                        reinterpret_cast<unsigned char const *>(msg.data()),
                        msg.size()},
                sig);
    }

}
