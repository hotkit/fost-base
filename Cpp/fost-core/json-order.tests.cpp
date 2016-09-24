/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/json-order.hpp>


using fostlib::json;
using fostlib::null;


FSL_TEST_SUITE(json_order);


namespace {
    template<typename J1, typename J2> inline
    bool order(const J1 &l, const J2 &r, bool e) {
        return std::less<json>()(json(l), json(r)) == e;
    }
}


FSL_TEST_FUNCTION(null) {
    FSL_CHECK(order(null, null, false));
    FSL_CHECK(order(null, false, true));
    FSL_CHECK(order(null, true, true));
    FSL_CHECK(order(null, -123, true));
    FSL_CHECK(order(null, 3.14, true));
    FSL_CHECK(order(null, fostlib::string(), true));
    FSL_CHECK(order(null, "string", true));
}


FSL_TEST_FUNCTION(boolean) {
    FSL_CHECK(order(false, null, false));
    FSL_CHECK(order(true, null, false));
    FSL_CHECK(order(false, false, false));
    FSL_CHECK(order(false, true, true));
    FSL_CHECK(order(false, null, false));
    FSL_CHECK(order(true, false, false));
    FSL_CHECK(order(true, true, false));
    FSL_CHECK(order(false, -123, true));
    FSL_CHECK(order(false, 0, true));
    FSL_CHECK(order(false, 123, true));
    FSL_CHECK(order(true, -123, true));
    FSL_CHECK(order(true, 0, true));
    FSL_CHECK(order(true, 123, true));
    FSL_CHECK(order(false, 3.14, true));
    FSL_CHECK(order(true, 3.14, true));
    FSL_CHECK(order(true, fostlib::string(), true));
    FSL_CHECK(order(true, "string", true));
}


FSL_TEST_FUNCTION(int) {
    FSL_CHECK(order(-123, null, false));
    FSL_CHECK(order(0, null, false));
    FSL_CHECK(order(123, null, false));
    FSL_CHECK(order(-123, false, false));
    FSL_CHECK(order(0, false, false));
    FSL_CHECK(order(123, false, false));
    FSL_CHECK(order(-123, true, false));
    FSL_CHECK(order(0, true, false));
    FSL_CHECK(order(123, true, false));
    FSL_CHECK(order(-123, -123, false));
    FSL_CHECK(order(-123, 0, true));
    FSL_CHECK(order(0, -123, false));
    FSL_CHECK(order(123, 0, false));
    FSL_CHECK(order(-123, 3.14, true));
    FSL_CHECK(order(0, 3.14, true));
    FSL_CHECK(order(123, 3.14, false));
    FSL_CHECK(order(123, fostlib::string(), true));
    FSL_CHECK(order(123, "string", true));
}


FSL_TEST_FUNCTION(double) {
    FSL_CHECK(order(3.14, null, false));
    FSL_CHECK(order(3.14, false, false));
    FSL_CHECK(order(3.14, true, false));
    FSL_CHECK(order(3.14, -123, false));
    FSL_CHECK(order(3.14, 0, false));
    FSL_CHECK(order(3.14, 123, true));
    FSL_CHECK(order(3.14, 1.6, false));
    FSL_CHECK(order(3.14, 6.28, true));
    FSL_CHECK(order(3.14, 3.14, false));
    FSL_CHECK(order(3.14, fostlib::string(), true));
    FSL_CHECK(order(3.14, "string", true));
}


FSL_TEST_FUNCTION(string) {
    FSL_CHECK(order(fostlib::string(), null, false));
    FSL_CHECK(order("string", null, false));
    FSL_CHECK(order(fostlib::string(), true, false));
    FSL_CHECK(order("string", true, false));
    FSL_CHECK(order(fostlib::string(), 123, false));
    FSL_CHECK(order("string", 123, false));
    FSL_CHECK(order(fostlib::string(), 3.14, false));
    FSL_CHECK(order("string", 3.14, false));
}

