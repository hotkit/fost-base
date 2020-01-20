/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"


FSL_TEST_SUITE(json_to_json);


FSL_TEST_FUNCTION(atoms) {
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(), false), "null");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(true), false), "true");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(false), false), "false");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(10), false), "10");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(0), false), "0");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(-10), false), "-10");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(3.141), false), "3.141");
    FSL_CHECK_EQ(
            fostlib::json::unparse(fostlib::json("Hello"), false), "\"Hello\"");
    FSL_CHECK_EQ(
            fostlib::json::unparse(fostlib::json("Bob's"), false), "\"Bob's\"");
    FSL_CHECK_EQ(
            fostlib::json::unparse(fostlib::json("Bob says \"Hello\""), false),
            "\"Bob says \\\"Hello\\\"\"");
}


FSL_TEST_FUNCTION(arrays) {
    fostlib::json a = fostlib::json::array_t();
    FSL_CHECK_EQ(fostlib::json::unparse(a, false), "[]");
    fostlib::jcursor().push_back(a, fostlib::json(1));
    fostlib::jcursor().push_back(a, fostlib::json("Hello"));
    FSL_CHECK_EQ(fostlib::json::unparse(a, false), "[1,\"Hello\"]");
}


FSL_TEST_FUNCTION(objects) {
    fostlib::json o = fostlib::json::object_t();
    FSL_CHECK_EQ(fostlib::json::unparse(o, false), "{}");
    fostlib::jcursor("key").insert(o, fostlib::json("value"));
    FSL_CHECK_EQ(fostlib::json::unparse(o, false), "{\"key\":\"value\"}");
}


FSL_TEST_FUNCTION(tortuous) {
    fostlib::json a = fostlib::json::array_t();
    fostlib::jcursor().push_back(a, fostlib::json(1));
    fostlib::jcursor().push_back(a, fostlib::json("Hello"));
    fostlib::json o = fostlib::json::object_t();
    fostlib::jcursor("key").insert(o, fostlib::json("value"));
    fostlib::jcursor("array").insert(o, a);
    FSL_CHECK_EQ(
            fostlib::json::unparse(o, false),
            "{\"array\":[1,\"Hello\"],\"key\":\"value\"}");
}


FSL_TEST_FUNCTION(json_output) {
    fostlib::stringstream ss;
    ss << fostlib::json();
}
