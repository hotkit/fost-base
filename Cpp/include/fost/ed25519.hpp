/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/memory.hpp>
#include <fost/array>


namespace fostlib {


    namespace ed25519 {


        /// A public or private key or other secret
        using secret = std::array<f5::byte, 32>;


        /// A keypair for ed25519
        struct keypair {
            secret priv, pub;

            /// Create a new keypair
            explicit keypair();

            /// Create a keypair from a secret
            explicit keypair(secret);
        };


    }


}

