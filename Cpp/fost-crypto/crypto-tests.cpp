/*
    Copyright 2008-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>


const fostlib::module fostlib::c_fost_crypto_test(c_fost_crypto, "test");


FSL_TEST_SUITE( crypto );


FSL_TEST_FUNCTION( sha1_hmac ) {
    // http://docs.amazonwebservices.com/AmazonS3/2006-03-01/RESTAuthentication.html
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n/johnsmith/photos/puppy.jpg"
    ), L"xXjDGYUmKxnwqr5KXNPGldn5LbA=" );
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"PUT\n\nimage/jpeg\nTue, 27 Mar 2007 21:15:45 +0000\n/johnsmith/photos/puppy.jpg"
    ), L"hcicpDDvL9SsO6AkvxqmIWkmOuQ=" );
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"GET\n\n\nTue, 27 Mar 2007 19:42:41 +0000\n/johnsmith/"
    ), L"jsRt/rhG+Vtp88HrYL706QhE4w4=" );
    FSL_CHECK_EQ( fostlib::sha1_hmac(
        L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o",
        L"PUT\n4gJE4saaMU4BqNR0kLY+lw==\napplication/x-download\nTue, 27 Mar 2007 21:06:08 +0000\nx-amz-acl:public-read\nx-amz-meta-checksumalgorithm:crc32\nx-amz-meta-filechecksum:0x02661779\nx-amz-meta-reviewedby:joe@johnsmith.net,jane@johnsmith.net\n/static.johnsmith.net/db-backup.dat.gz"
    ), L"C0FlOtU8Ylb9KDTpZqYkZPX91iI=" );
}

FSL_TEST_FUNCTION( sha1_hmac_1 ) {
    fostlib::hmac signature(fostlib::sha1, L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o");
    signature << fostlib::string(L"GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n/johnsmith/photos/puppy.jpg");
    FSL_CHECK_EQ(fostlib::coerce< fostlib::base64_string >( signature.digest() ), fostlib::base64_string("xXjDGYUmKxnwqr5KXNPGldn5LbA="));
}

FSL_TEST_FUNCTION( sha1_hmac_2 ) {
    fostlib::hmac signature(fostlib::sha1, L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o");
    signature << fostlib::string(L"GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n");
    signature << fostlib::string(L"/johnsmith/photos/puppy.jpg");
    FSL_CHECK_EQ(fostlib::coerce< fostlib::base64_string >( signature.digest() ), fostlib::base64_string("xXjDGYUmKxnwqr5KXNPGldn5LbA="));
}

FSL_TEST_FUNCTION( sha1_hmac_3 ) {
    fostlib::hmac signature(fostlib::sha1, L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o");
    signature << "GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n";
    signature << "/johnsmith/photos/puppy.jpg";
    FSL_CHECK_EQ(fostlib::coerce< fostlib::base64_string >( signature.digest() ), fostlib::base64_string("xXjDGYUmKxnwqr5KXNPGldn5LbA="));
}

FSL_TEST_FUNCTION( sha1_hmac_4 ) {
    fostlib::hmac signature(fostlib::sha1, L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o");
    const char *l1 = "GET\n\n\nTue, 27 Mar 2007 19:36:42 +0000\n";
    signature << std::make_pair(
        reinterpret_cast< const unsigned char * >( l1 ),
        reinterpret_cast< const unsigned char * >( l1 + 38 )
    );
    signature << std::vector<unsigned char>{'/', 'j', 'o', 'h', 'n', 's', 'm', 'i', 't', 'h'};
    signature << "/photos/puppy.jpg";
    FSL_CHECK_EQ(fostlib::coerce< fostlib::base64_string >( signature.digest() ), fostlib::base64_string("xXjDGYUmKxnwqr5KXNPGldn5LbA="));
}

FSL_TEST_FUNCTION( sha1_hmac_5 ) {
    fostlib::hmac signature(fostlib::sha1, L"uV3F3YluFJax1cknvbcGwgjvx4QpvB+leU8dUj2o");
    signature << fostlib::string(L"GET") << "\n";
    signature << "\n\n";
    signature << fostlib::string(L"Tue, 27 Mar 2007 19:36:42 +0000") << "\n";
    signature << "/" << fostlib::string(L"johnsmith") << fostlib::string(L"/photos/puppy.jpg");
    FSL_CHECK_EQ(fostlib::coerce< fostlib::base64_string >( signature.digest() ), fostlib::base64_string("xXjDGYUmKxnwqr5KXNPGldn5LbA="));
}

// Values from http://en.wikipedia.org/wiki/SHA-2
FSL_TEST_FUNCTION( sha256 ) {
    FSL_CHECK_EQ(fostlib::sha256(""),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

// Values from http://en.wikipedia.org/wiki/Hash-based_message_authentication_code
FSL_TEST_FUNCTION( sha256_hmac_1a ) {
    fostlib::hmac signature(fostlib::sha256, "");
    signature << "";
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( signature.digest() ),
        "b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad");
}
FSL_TEST_FUNCTION( sha256_hmac_1b ) {
    std::array<unsigned char, 0> secret;
    fostlib::hmac signature(fostlib::sha256, secret);
    signature << "";
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( signature.digest() ),
        "b613679a0814d9ec772f95d778c35fc5ff1697c493715653c6c712144292c5ad");
}
FSL_TEST_FUNCTION( sha256_hmac_2a ) {
    fostlib::hmac signature(fostlib::sha256, "key");
    signature << "The quick brown fox jumps over the lazy dog";
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( signature.digest() ),
        "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8");
}
FSL_TEST_FUNCTION( sha256_hmac_2b ) {
    std::array<unsigned char, 3> secret {{'k', 'e', 'y'}};
    fostlib::hmac signature(fostlib::sha256, secret);
    signature << "The quick brown fox jumps over the lazy dog";
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( signature.digest() ),
        "f7bc83f430538424b13298e6aa6fb143ef4d59a14946175997479dbc2d1a3cd8");
}
