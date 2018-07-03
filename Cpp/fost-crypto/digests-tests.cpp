/**
    Copyright 2008-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-crypto-test.hpp"
#include <fost/crypto>
#include <fost/unicode>


FSL_TEST_SUITE( digests );


/**
    ## sha1
 */


FSL_TEST_FUNCTION( sha1_empty ) {
    fostlib::digester ex1( fostlib::sha1 );
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("da39a3ee5e6b4b0d3255bfef95601890afd80709"));
}
FSL_TEST_FUNCTION( sha1_string ) {
    fostlib::digester ex1( fostlib::sha1 ), ex2( fostlib::sha1 );
    ex1 << fostlib::string("The quick brown fox jumps over the lazy dog");
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"));
    ex2 << fostlib::string("The quick brown fox jumps over the lazy cog");
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex2.digest() ),
        fostlib::hex_string("de9f2c7fd25e1b3afad3e85a0bd17d9b100db4b3"));
}
FSL_TEST_FUNCTION( sha1_parts ) {
    fostlib::digester ex1( fostlib::sha1 );
    ex1 << fostlib::string("The qu");
    ex1 << fostlib::string("ick brown fox jumps over the lazy dog");
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"));
}
FSL_TEST_FUNCTION(sha1_memory) {
    const char s1[] = "The quick brown fox jumps over the lazy dog";
    const char s2[] = "The quick brown fox jumps over the lazy dog";
    fostlib::digester ex1(fostlib::sha1), ex2(fostlib::sha1);
    ex1 << fostlib::const_memory_block(s1, s1 + sizeof(s1) - 1);
    ex2 << fostlib::const_memory_block(s2, s2 + sizeof(s2) - 1);
    FSL_CHECK_EQ(
        fostlib::coerce<fostlib::hex_string>(ex1.digest()),
        fostlib::hex_string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"));
    FSL_CHECK_EQ(
        fostlib::coerce<fostlib::hex_string>(ex2.digest()),
        fostlib::hex_string("2fd4e1c67a2d28fced849ee1bb76e7391b93eb12"));
}


/**
    ## ripemd256
 */


FSL_TEST_FUNCTION(ripemd256_empty) {
    fostlib::digester ex1(fostlib::ripemd256);
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("02ba4c4e5f8ecd1877fc52d64d30e37a2d9774fb1e5d026380ae0168e3c5522d"));
}


/**
    ## md5
 */


FSL_TEST_FUNCTION( md5_empty ) {
    fostlib::digester ex1( fostlib::md5 );
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("d41d8cd98f00b204e9800998ecf8427e"));
}
FSL_TEST_FUNCTION( md5_string ) {
    fostlib::digester ex1( fostlib::md5 ), ex2( fostlib::md5 );
    ex1 << fostlib::string("The quick brown fox jumps over the lazy dog");
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("9e107d9d372bb6826bd81d3542a419d6"));
    ex2 << fostlib::string("The quick brown fox jumps over the lazy dog.");
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex2.digest() ),
        fostlib::hex_string("e4d909c290d0fb1ca068ffaddf22cbd0"));
}
FSL_TEST_FUNCTION( md5_parts ) {
    fostlib::digester ex1( fostlib::md5 );
    ex1 << fostlib::string("The qu");
    ex1 << fostlib::string("ick brown fox jumps over the ");
    ex1 << std::vector<unsigned char>{'l', 'a', 'z', 'y'};
    const char *last = " dog";
    ex1 << std::make_pair(last, last + 4);
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::hex_string >( ex1.digest() ),
        fostlib::hex_string("9e107d9d372bb6826bd81d3542a419d6"));
}

