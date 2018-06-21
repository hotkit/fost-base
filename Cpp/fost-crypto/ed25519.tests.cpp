/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/crypto>
#include <fost/ed25519.hpp>
#include <fost/push_back>
#include <fost/test>


FSL_TEST_SUITE(ed25519);


FSL_TEST_FUNCTION(new_keys) {
    fostlib::ed25519::keypair keys;
}


FSL_TEST_FUNCTION(public_from_private) {
    const fostlib::ed25519::secret priv{{
        f5::byte(0x9d), f5::byte(0x61), f5::byte(0xb1), f5::byte(0x9d),
        f5::byte(0xef), f5::byte(0xfd), f5::byte(0x5a), f5::byte(0x60),
        f5::byte(0xba), f5::byte(0x84), f5::byte(0x4a), f5::byte(0xf4),
        f5::byte(0x92), f5::byte(0xec), f5::byte(0x2c), f5::byte(0xc4),
        f5::byte(0x44), f5::byte(0x49), f5::byte(0xc5), f5::byte(0x69),
        f5::byte(0x7b), f5::byte(0x32), f5::byte(0x69), f5::byte(0x19),
        f5::byte(0x70), f5::byte(0x3b), f5::byte(0xac), f5::byte(0x03),
        f5::byte(0x1c), f5::byte(0xae), f5::byte(0x7f), f5::byte(0x60)}};
    const fostlib::ed25519::secret pub{{
        f5::byte(0xd7), f5::byte(0x5a), f5::byte(0x98), f5::byte(0x01),
        f5::byte(0x82), f5::byte(0xb1), f5::byte(0x0a), f5::byte(0xb7),
        f5::byte(0xd5), f5::byte(0x4b), f5::byte(0xfe), f5::byte(0xd3),
        f5::byte(0xc9), f5::byte(0x64), f5::byte(0x07), f5::byte(0x3a),
        f5::byte(0x0e), f5::byte(0xe1), f5::byte(0x72), f5::byte(0xf3),
        f5::byte(0xda), f5::byte(0xa6), f5::byte(0x23), f5::byte(0x25),
        f5::byte(0xaf), f5::byte(0x02), f5::byte(0x1a), f5::byte(0x68),
        f5::byte(0xf7), f5::byte(0x07), f5::byte(0x51), f5::byte(0x1a)}};

    fostlib::ed25519::keypair keys{priv};
    try {
        FSL_CHECK(keys.pub() == pub);
    } catch ( fostlib::exceptions::exception &e ) {
        for ( auto c : keys.pub() ) {
            fostlib::push_back(e.data(), "public", "generated", int(c));
        }
        for ( auto c : pub ) {
            fostlib::push_back(e.data(), "public", "expected", int(c));
        }
        throw;
    }
}

