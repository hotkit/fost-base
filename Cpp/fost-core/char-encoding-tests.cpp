/**
    Copyright 2009-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"
#include <fost/unicode>

#include <fost/exception/unicode_encoding.hpp>


using namespace fostlib::utf;


FSL_TEST_SUITE(char_encoding);


FSL_TEST_FUNCTION(lengths_narrow) {
    FSL_CHECK_EQ(length(""), 0u);
    FSL_CHECK_EQ(length("a"), 1u);
}

FSL_TEST_FUNCTION(lengths_utf8) {
    FSL_CHECK_EQ(utf8length(0x20), 1u); // space
    FSL_CHECK_EQ(utf8length(0xe5), 2u); // latin small letter a with ring above
    FSL_CHECK_EQ(utf8length(0x3b3), 2u); // greek small letter gamma
    FSL_CHECK_EQ(utf8length(0x2014), 3u); // em dash
    FSL_CHECK_EQ(utf8length(0xffe1), 3u); // full width pound sign
    FSL_CHECK_EQ(utf8length(0x1d11e), 4u); // treble clef
}

FSL_TEST_FUNCTION(lengths_utf16) {
    FSL_CHECK_EQ(utf16length(0x20), 1u); // space
    FSL_CHECK_EQ(utf16length(0xe5), 1u); // latin small letter a with ring above
    FSL_CHECK_EQ(utf16length(0x3b3), 1u); // greek small letter gamma
    FSL_CHECK_EQ(utf16length(0x2014), 1u); // em dash
    FSL_CHECK_EQ(utf16length(0xffe1), 1u); // full width pound sign
    FSL_CHECK_EQ(utf16length(0x1d11e), 2u); // treble clef
}


FSL_TEST_FUNCTION(coerce) {
    char const *const s = "\xc3\xa6";
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(fostlib::ascii_string("abc")),
            "abc");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(fostlib::utf8_string("abc")),
            "abc");
    FSL_CHECK_EQ(
            fostlib::coerce<fostlib::string>(fostlib::utf8_string(s)),
            u"\x00e6");
    FSL_CHECK_EQ(
            fostlib::utf8_string("S") + fostlib::utf8_string(s)
                    + fostlib::utf8_string("lensminde"),
            fostlib::utf8_string{f5::u8string{u"S\x00e6lensminde"}});
}
