 /*
    Copyright 2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

using namespace f5::literals;


FSL_TEST_SUITE(json_coerce);


FSL_TEST_FUNCTION(nullable) {
    FSL_CHECK_EQ(
        fostlib::coerce<fostlib::json>(fostlib::nullable<fostlib::string>()), fostlib::json());
    FSL_CHECK_EQ(
        fostlib::coerce<fostlib::json>(fostlib::nullable<fostlib::string>("t")), fostlib::json("t"));
}


FSL_TEST_FUNCTION(bool) {
    FSL_CHECK(not fostlib::coerce<bool>(fostlib::json("")));
    FSL_CHECK(fostlib::coerce<bool>(fostlib::json("x")));
    FSL_CHECK(not fostlib::coerce<bool>(fostlib::json(""_l)));
    FSL_CHECK(fostlib::coerce<bool>(fostlib::json("x"_l)));
}


FSL_TEST_FUNCTION(int64_t) {
    FSL_CHECK_EQ(fostlib::coerce<int64_t>("123"_l), int64_t(123));
}


FSL_TEST_FUNCTION(object) {
    FSL_CHECK_EQ(fostlib::json(fostlib::json::object_t()),
        fostlib::coerce<fostlib::json>(fostlib::json::object_t()));
}


FSL_TEST_FUNCTION(array) {
    FSL_CHECK_EQ(fostlib::json(fostlib::json::array_t()),
        fostlib::coerce<fostlib::json>(fostlib::json::array_t()));
}

