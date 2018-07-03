/**
    Copyright 2008-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>


FSL_TEST_SUITE(hashes);


FSL_TEST_FUNCTION(sha1) {
    /// Values from <http://en.wikipedia.org/wiki/Examples_of_SHA_digests>
    FSL_CHECK_EQ( fostlib::sha1(fostlib::string()),
        "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    FSL_CHECK_EQ( fostlib::sha1("The quick brown fox jumps over the lazy dog" ),
        "2fd4e1c67a2d28fced849ee1bb76e7391b93eb12");
    FSL_CHECK_EQ( fostlib::sha1("The quick brown fox jumps over the lazy cog" ),
        "de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3");
}


FSL_TEST_FUNCTION(sha256) {
    /// Value from <http://en.wikipedia.org/wiki/SHA-2>
    FSL_CHECK_EQ(fostlib::sha256(""),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    /// Value from <http://www.febooti.com/products/filetweak/members/hash-and-crc/test-vectors/>
    FSL_CHECK_EQ(fostlib::sha256("The quick brown fox jumps over the lazy dog"),
        "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592");
}


FSL_TEST_FUNCTION(ripemd256) {
    /// Value from <http://www.febooti.com/products/filetweak/members/hash-and-crc/test-vectors/>
    FSL_CHECK_EQ(fostlib::ripemd256(""),
        "02ba4c4e5f8ecd1877fc52d64d30e37a2d9774fb1e5d026380ae0168e3c5522d");
    FSL_CHECK_EQ(fostlib::ripemd256("The quick brown fox jumps over the lazy dog"),
        "c3b0c2f764ac6d576a6c430fb61a6f2255b4fa833e094b1ba8c1e29b6353036f");
    /// Value from internal test data
    FSL_CHECK_EQ(fostlib::ripemd256("1234----ProjectC534---/4y2krF38"),
        "508f1795d9342d12a5b9cfa4c3c181cc0d3f6abbe974001534de3b5b014ccee0");
}

