/**
    Copyright 2001-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/

#include <fost/test>
#include <fost/unicode>

#define TRIM(X)(fostlib::trim(fostlib::nullable<f5::u8view>(X)))

FSL_TEST_SUITE(trim);

FSL_TEST_FUNCTION(basic_trim) {
    FSL_CHECK_EQ(TRIM(), fostlib::null);
    FSL_CHECK_EQ(TRIM("\u0020"), fostlib::null);
    FSL_CHECK_EQ(TRIM("\uFEFFstr\u2007ing\u205F\u202F"), "str\u2007ing");
    FSL_CHECK_EQ(TRIM("string\u2002\u2003\u2004"), "string");
    FSL_CHECK_EQ(TRIM("\u2001\u00A0string"), "string");
    FSL_CHECK_EQ(TRIM("\r\t\u0020string\n\u0020\f"), "string");
    FSL_CHECK_EQ(TRIM("\u2000string\u3000\u2008"), "string");
}