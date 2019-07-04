/**
    Copyright 2001-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/

#include <fost/test>
#include <fost/unicode>

#define TRIM(X)(fostlib::trim(fostlib::nullable<f5::u8view>{X}))

FSL_TEST_SUITE(trim);

FSL_TEST_FUNCTION(basic_trim) {
    FSL_CHECK_EQ(TRIM(), fostlib::null);
    FSL_CHECK_EQ(TRIM(""), fostlib::null);
    FSL_CHECK_EQ(TRIM("\x20\xE2\x80\x8A"), fostlib::null);
    FSL_CHECK_EQ(TRIM("\xEF\xBB\xBFstr\xE2\x80\x87ing\xE2\x81\x9F\xE2\x80\xAF"), "str\xE2\x80\x87ing");
    FSL_CHECK_EQ(TRIM("string\xE2\x80\x82\xE2\x80\x83\xE2\x80\x84"), "string");
    FSL_CHECK_EQ(TRIM("\xE2\x80\x81\xC2\xA0string"), "string");
    FSL_CHECK_EQ(TRIM("\r\t\x20string\n\x20\f"), "string");
    FSL_CHECK_EQ(TRIM("\xE2\x80\x80string\xE3\x80\x80\xE2\x80\x88"), "string");
    FSL_CHECK_EQ(fostlib::trim(fostlib::nullable<fostlib::ascii_string>{" abc "}), "abc");
}