/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>


/**
 https://jwt.io/ can be used to verify test results.
 */

FSL_TEST_SUITE(jwt);


FSL_TEST_FUNCTION(hs256_empty) {
    fostlib::jwt::mint minter(fostlib::sha256, "secret");
    FSL_CHECK_EQ(minter.token(),
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "e30=."
            "JWUVuXQuNrdh4xCw79gyFq+Jf95aPrzPZI2GV5GpoQI=");
}

