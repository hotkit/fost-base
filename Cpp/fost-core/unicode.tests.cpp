/*
    Copyright 2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE(unicode);


FSL_TEST_FUNCTION(u16iter_basic) {
    fostlib::utf8_string s("abc 123");
    auto u8v = f5::u8view(s);
    auto pos = u8v.u16begin();
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, 'a');
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, 'b');
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, 'c');
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, ' ');
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, '1');
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, '2');
    FSL_CHECK(pos != u8v.u16end());
    FSL_CHECK_EQ(*pos++, '3');
    FSL_CHECK(pos == u8v.u16end());
}


FSL_TEST_FUNCTION(u16iter_empty) {
    fostlib::utf8_string s;
    auto u8v = f5::u8view(s);
    auto pos = u8v.u16begin();
    FSL_CHECK(pos == u8v.u16end());
}


FSL_TEST_FUNCTION(u16iter_pair) {
    auto check = [](f5::utf32 cp, f5::utf16 c1, f5::utf16 c2) {
            fostlib::string s;
            s += f5::utf32(cp);
            FSL_CHECK_EQ(*s.begin(), cp);
            auto u8 = fostlib::coerce<fostlib::utf8_string>(s);
            auto u8v = f5::u8view(u8);
            FSL_CHECK_EQ(*u8v.begin(), cp);
            auto pos = u8v.u16begin();
            FSL_CHECK(pos != u8v.u16end());
            FSL_CHECK_EQ(*pos++, c1);
            FSL_CHECK(pos != u8v.u16end());
            FSL_CHECK_EQ(*pos++, c2);
            FSL_CHECK(pos == u8v.u16end());
        };
    check(0x10348, 0xD800, 0xDF48);
    check(0x1d11e, 0xd834, 0xdd1e);
}

