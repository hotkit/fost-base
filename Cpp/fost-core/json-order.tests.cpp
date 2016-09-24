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
}


FSL_TEST_FUNCTION(boolean) {
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
}


FSL_TEST_FUNCTION(int) {
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
}

