/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>

#include <fost/push_back>


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


FSL_TEST_FUNCTION(hs256_sub) {
    fostlib::jwt::mint minter(fostlib::sha256, "secret");
    minter.subject("hello");
    FSL_CHECK_EQ(minter.token(),
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJzdWIiOiJoZWxsbyJ9."
            "YMxTsTS6Ndzb9IXjVoGrSrYcIFVd09WtLufoQjAGkaw=");
}


FSL_TEST_FUNCTION(hs256_claim) {
    fostlib::jwt::mint minter(fostlib::sha256, "secret");
    fostlib::json groups;
    fostlib::push_back(groups, "g1");
    fostlib::push_back(groups, "g2");
    minter.claim("http://example.com/groups", groups);
    FSL_CHECK_EQ(minter.token(),
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJodHRwOi8vZXhhbXBsZS5jb20vZ3JvdXBzIjpbImcxIiwiZzIiXX0=."
            "u4jh8PW3e/1FNwfLBmtbI8X7KNF0hn/aVZKDcWlNgaM=");
}

