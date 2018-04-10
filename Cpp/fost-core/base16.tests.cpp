/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/base16>
#include <fost/test>


FSL_TEST_SUITE(base16);


FSL_TEST_FUNCTION(to_bytes) {
    fostlib::base16_string b1{"1234"};
    auto v1 = fostlib::coerce<std::vector<unsigned char>>(b1);
    FSL_CHECK_EQ(v1[0], 0x12);
    FSL_CHECK_EQ(v1[1], 0x34);
}


FSL_TEST_FUNCTION(odd_char_count) {
    fostlib::base16_string b1{"123"};
    FSL_CHECK_EXCEPTION(
        fostlib::coerce<std::vector<unsigned char>>(b1),
        fostlib::exceptions::parse_error&);
}


FSL_TEST_FUNCTION(illegal_char) {
    FSL_CHECK_EXCEPTION(
        fostlib::base16_string{"123X34"},
        fostlib::exceptions::parse_error&);
}

