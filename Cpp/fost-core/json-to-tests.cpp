/**
    Copyright 2008-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"


FSL_TEST_SUITE(json_to_json);


FSL_TEST_FUNCTION(atoms) {
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(), false), L"null");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(true), false), L"true");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(false), false), L"false");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(10), false), L"10");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(0), false), L"0");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(-10), false), L"-10");
    FSL_CHECK_EQ(fostlib::json::unparse(fostlib::json(3.141), false), L"3.141");
    FSL_CHECK_EQ(
            fostlib::json::unparse(fostlib::json(L"Hello"), false),
            L"\"Hello\"");
    FSL_CHECK_EQ(
            fostlib::json::unparse(fostlib::json(L"Bob's"), false),
            L"\"Bob's\"");
    FSL_CHECK_EQ(
            fostlib::json::unparse(fostlib::json(L"Bob says \"Hello\""), false),
            L"\"Bob says \\\"Hello\\\"\"");
}


FSL_TEST_FUNCTION(arrays) {
    fostlib::json a = fostlib::json::array_t();
    FSL_CHECK_EQ(fostlib::json::unparse(a, false), L"[]");
    fostlib::jcursor().push_back(a, fostlib::json(1));
    fostlib::jcursor().push_back(a, fostlib::json(L"Hello"));
    FSL_CHECK_EQ(fostlib::json::unparse(a, false), L"[1,\"Hello\"]");
}


FSL_TEST_FUNCTION(objects) {
    fostlib::json o = fostlib::json::object_t();
    FSL_CHECK_EQ(fostlib::json::unparse(o, false), L"{}");
    fostlib::jcursor(L"key").insert(o, fostlib::json(L"value"));
    FSL_CHECK_EQ(fostlib::json::unparse(o, false), L"{\"key\":\"value\"}");
}


FSL_TEST_FUNCTION(tortuous) {
    fostlib::json a = fostlib::json::array_t();
    fostlib::jcursor().push_back(a, fostlib::json(1));
    fostlib::jcursor().push_back(a, fostlib::json(L"Hello"));
    fostlib::json o = fostlib::json::object_t();
    fostlib::jcursor(L"key").insert(o, fostlib::json(L"value"));
    fostlib::jcursor(L"array").insert(o, a);
    FSL_CHECK_EQ(
            fostlib::json::unparse(o, false),
            L"{\"array\":[1,\"Hello\"],\"key\":\"value\"}");
}


FSL_TEST_FUNCTION(json_output) {
    fostlib::stringstream ss;
    ss << fostlib::json();
}
