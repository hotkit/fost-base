/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/base32>
#include <fost/test>


FSL_TEST_SUITE(base32);


FSL_TEST_FUNCTION(bytes_1) {
    const std::array<f5::byte, 1> a{{f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "74======");
}


FSL_TEST_FUNCTION(bytes_2) {
    const std::array<f5::byte, 2> a{{f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "777Q====");
}


FSL_TEST_FUNCTION(bytes_3) {
    const std::array<f5::byte, 3> a{{f5::byte(0xff), f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "77776===");
}


FSL_TEST_FUNCTION(bytes_4) {
    const std::array<f5::byte, 4> a{{f5::byte(0xff), f5::byte(0xff), f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "777777Y=");
}


FSL_TEST_FUNCTION(bytes_5) {
    const std::array<f5::byte, 5> a{{f5::byte(0xff), f5::byte(0xff), f5::byte(0xff), f5::byte(0xff), f5::byte(0xff)}};
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32_string>(a), "77777777");
}


FSL_TEST_FUNCTION(bytes_long) {
    const f5::lstring foobar = "foobar";
    FSL_CHECK_EQ(fostlib::coerce<fostlib::base32hex_string>(foobar), "CPNMUOJ1E8======");
}

