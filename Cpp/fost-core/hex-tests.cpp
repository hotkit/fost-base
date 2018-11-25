/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/string>

#include <fost/exception/parse_error.hpp>


using namespace fostlib;


FSL_TEST_SUITE(hex);


FSL_TEST_FUNCTION(constructors) {
    FSL_CHECK_NOTHROW(hex_string());
    FSL_CHECK_NOTHROW(hex_string("0123456789abcdef"));
    FSL_CHECK_EXCEPTION(
            hex_string("0123456789 abcdef"), exceptions::parse_error &);
}


FSL_TEST_FUNCTION(raw_array) {
    boost::array<unsigned char, 9> values;
    for (std::size_t p = 0; p != 9; ++p) values[p] = p + 1;
    FSL_CHECK_EQ(coerce<hex_string>(values), hex_string("010203040506070809"));
}


FSL_TEST_FUNCTION(unsigned_char) {
    typedef unsigned char uc;
    FSL_CHECK_EQ(coerce<hex_string>(uc(0)), hex_string("00"));
    FSL_CHECK_EQ(coerce<hex_string>(uc(127)), hex_string("7f"));
    FSL_CHECK_EQ(coerce<hex_string>(uc(255)), hex_string("ff"));
}


FSL_TEST_FUNCTION(unsigned_char_arrays) { std::vector<unsigned char> v; }


FSL_TEST_FUNCTION(hex_parser) {
    FSL_CHECK_EQ(coerce<std::size_t>(hex_string("f")), 15u);
    FSL_CHECK_EQ(coerce<std::size_t>(hex_string("ff")), 255u);
    FSL_CHECK_EQ(coerce<std::size_t>(hex_string("10000")), 65536u);
}
