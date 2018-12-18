/**
    Copyright 2016-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>

#include <fost/push_back>


/**
    <https://jwt.io/> can be used to verify test results.
 */

FSL_TEST_SUITE(jwt);


FSL_TEST_FUNCTION(hs256_empty_mint) {
    fostlib::jwt::mint minter(fostlib::jwt::alg::HS256);
    FSL_CHECK_EQ(
            minter.token(f5::u8view{"secret"}.memory()),
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "e30."
            "t-IDcSemACt8x4iTMCda8Yhe3iZaWbvV5XKSTbuAn0M");
}
FSL_TEST_FUNCTION(hs256_empty_verify) {
    auto jwt = fostlib::jwt::token::load(
            "secret",
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "e30."
            "t-IDcSemACt8x4iTMCda8Yhe3iZaWbvV5XKSTbuAn0M");
    FSL_CHECK(jwt);
    FSL_CHECK_EQ(jwt.value().header["typ"], fostlib::json("JWT"));
    FSL_CHECK_EQ(jwt.value().payload, fostlib::json::object_t());
}


FSL_TEST_FUNCTION(hs256_sub) {
    fostlib::jwt::mint minter(fostlib::jwt::alg::HS256);
    minter.subject("hello");
    FSL_CHECK_EQ(
            minter.token(f5::u8view{"secret"}.memory()),
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJzdWIiOiJoZWxsbyJ9."
            "YMxTsTS6Ndzb9IXjVoGrSrYcIFVd09WtLufoQjAGkaw");
}


FSL_TEST_FUNCTION(hs256_claim) {
    fostlib::jwt::mint minter(fostlib::jwt::alg::HS256);
    fostlib::json groups;
    fostlib::push_back(groups, "g1");
    fostlib::push_back(groups, "g2");
    minter.claim("http://example.com/groups", groups);
    FSL_CHECK_EQ(
            minter.token(f5::u8view{"secret"}.memory()),
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJodHRwOi8vZXhhbXBsZS5jb20vZ3JvdXBzIjpbImcxIiwiZzIiXX0."
            "04gmC8hnktHdDJjCmrON637xAW7jhF86IhqeZUv7Wwg");
}


FSL_TEST_FUNCTION(verify_not_base64) {
    FSL_CHECK(not fostlib::jwt::token::load("secret", "AB@_.ABads.advbew"));
}
FSL_TEST_FUNCTION(verify_not_signed) {
    FSL_CHECK(not fostlib::jwt::token::load(
            "secret", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.e30"));
}
FSL_TEST_FUNCTION(verify_not_jwt) {
    FSL_CHECK(not fostlib::jwt::token::load("secret", "e30.e30.abasd"));
}
FSL_TEST_FUNCTION(verify_not_hs256) {
    FSL_CHECK(not fostlib::jwt::token::load(
            "secret", "eyJ0eXAiOiJKV1QifQ.e30.abasd"));
}
FSL_TEST_FUNCTION(verify_wrong_signature) {
    FSL_CHECK(not fostlib::jwt::token::load(
            "secret",
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJodHRwOi8vZXhhbXBsZS5jb20vZ3JvdXBzIjpbImcxIiwiZzIiXX0."
            "u4jh8PW3e/1FNwfLBmtbI8X7KNF0hn/aVZKDcWlNgaT"));
    FSL_CHECK(not fostlib::jwt::token::load(
            "secret",
            "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9."
            "eyJleHAiOjE1MjkwNjM4NDksInN1YiI6ImFjY2Vzc0FkbWluIiwibmF"
            "tZSI6ImFjY2Vzc0FkbWluIn0."
            "ixIdoEQfZr0_2tPpAPhvlcvCQz9Uh3f5cJGmmP_2_yQ"));
}
FSL_TEST_FUNCTION(verify_expired) {
    FSL_CHECK(not fostlib::jwt::token::load(
            "secret",
            "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
            "eyJleHAiOjEzNjkyNTI1NTksIm5hbWUiOiJTeXN0ZW0gYWRtaW4iLCJ"
            "zdWIiOiJyb290In0."
            "FreR5QYJMJN0SyQvc5iUHbxL9OvWUQWOEXmUMdXystk"));
}
