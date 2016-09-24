/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/json-order.hpp>
#include <fost/push_back.hpp>


using fostlib::json;
using fostlib::null;


FSL_TEST_SUITE(json_order);


namespace {
    template<typename J1, typename J2> inline
    bool order(const J1 &l, const J2 &r, bool e) {
        return std::less<json>()(json(l), json(r)) == e;
    }

    json small_array(json ret = json::array_t()) {
        fostlib::push_back(ret, null);
        fostlib::push_back(ret, 123);
        fostlib::push_back(ret, "string");
        return ret;
    }
    json big_array(json ret = json::array_t()) {
        fostlib::push_back(ret, "string");
        fostlib::push_back(ret, null);
        fostlib::push_back(ret, 123);
        return ret;
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
    FSL_CHECK(order(null, json::array_t(), true));
    FSL_CHECK(order(null, big_array(), true));
    FSL_CHECK(order(null, json::object_t(), true));
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
    FSL_CHECK(order(true, json::array_t(), true));
    FSL_CHECK(order(true, big_array(), true));
    FSL_CHECK(order(true, json::object_t(), true));
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
    FSL_CHECK(order(123, json::array_t(), true));
    FSL_CHECK(order(123, big_array(), true));
    FSL_CHECK(order(123, json::object_t(), true));
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
    FSL_CHECK(order(3.14, json::array_t(), true));
    FSL_CHECK(order(3.14, big_array(), true));
    FSL_CHECK(order(3.14, json::object_t(), true));
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
    FSL_CHECK(order(fostlib::string(), fostlib::string(), false));
    FSL_CHECK(order(fostlib::string(), "string", true));
    FSL_CHECK(order("string", fostlib::string(), false));
    FSL_CHECK(order("string", "string", false));
    FSL_CHECK(order(fostlib::string(), json::array_t(), true));
    FSL_CHECK(order(fostlib::string(), big_array(), true));
    FSL_CHECK(order(fostlib::string(), json::object_t(), true));
}


FSL_TEST_FUNCTION(array) {
    FSL_CHECK(order(json::array_t(), null, false));
    FSL_CHECK(order(json::array_t(), true, false));
    FSL_CHECK(order(json::array_t(), false, false));
    FSL_CHECK(order(json::array_t(), 123, false));
    FSL_CHECK(order(json::array_t(), 3.14, false));
    FSL_CHECK(order(json::array_t(), fostlib::string(), false));
    FSL_CHECK(order(json::array_t(), "string", false));
    FSL_CHECK(order(json::array_t(), json::array_t(), false));
    FSL_CHECK(order(json::array_t(), small_array(), true));
    FSL_CHECK(order(json::array_t(), big_array(), true));
    FSL_CHECK(order(small_array(), json::array_t(), false));
    FSL_CHECK(order(small_array(), small_array(), false));
    FSL_CHECK(order(small_array(), big_array(), true));
    FSL_CHECK(order(big_array(), json::array_t(), false));
    FSL_CHECK(order(big_array(), small_array(), false));
    FSL_CHECK(order(big_array(), big_array(), false));
    FSL_CHECK(order(big_array(), big_array(big_array()), true));
    FSL_CHECK(order(json::array_t(), json::object_t(), true));
}


FSL_TEST_FUNCTION(object) {
    FSL_CHECK(order(json::object_t(), null, false));
}

