/**
    Copyright 2008-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core-test.hpp"

#include <fost/exception/json_error.hpp>
#include <fost/exception/not_null.hpp>


FSL_TEST_SUITE(json_object);


FSL_TEST_FUNCTION(constructors) {
    FSL_CHECK(fostlib::json(fostlib::json::object_t()).isobject());
}


FSL_TEST_FUNCTION(iteration) {
    fostlib::json o;
    fostlib::insert(o, "key", "value");
    for (const auto &p : o.object()) {
        FSL_CHECK_EQ(p.first, "key");
        FSL_CHECK_EQ(p.second, "value");
    }
}


FSL_TEST_FUNCTION(fetching) {
    fostlib::json a = fostlib::json::object_t();
    FSL_CHECK_EQ(a[L"nokey"], fostlib::json());
    fostlib::jcursor(L"key").insert(a, fostlib::json(L"value"));
    FSL_CHECK_EQ(a[L"key"], fostlib::json(L"value"));

    FSL_CHECK_EXCEPTION(
            fostlib::json()[L"nokey"], fostlib::exceptions::json_error &);
    fostlib::json array = fostlib::json::array_t();
    FSL_CHECK_EXCEPTION(array[L"nokey"], fostlib::exceptions::json_error &);

    fostlib::string key("\u0079");
    FSL_CHECK(not a.has_key(key));
    FSL_CHECK_EQ(a[key], fostlib::json());
}


FSL_TEST_FUNCTION(insert) {
    fostlib::json a = fostlib::json::object_t();
    fostlib::jcursor(L"key").insert(a, fostlib::json(L"value"));
    FSL_CHECK_EQ(a.size(), 1u);
    FSL_CHECK_EQ(a[L"key"], fostlib::json(L"value"));
    fostlib::jcursor(L"key 2").insert(a, fostlib::json(10));
    FSL_CHECK_EXCEPTION(
            fostlib::jcursor(L"key 2").insert(a, fostlib::json(12)),
            fostlib::exceptions::not_null &);
    FSL_CHECK_EQ(a.size(), 2u);
    FSL_CHECK_EQ(a[L"key 2"], fostlib::json(10));
}


FSL_TEST_FUNCTION(set) {
    fostlib::json a = fostlib::json::object_t();
    FSL_CHECK_NOTHROW(fostlib::jcursor(L"key")(a) = fostlib::json(L"value"));
    FSL_CHECK_EQ(a.size(), 1u);
    FSL_CHECK_EQ(a[L"key"], fostlib::json(L"value"));
    FSL_CHECK_NOTHROW(fostlib::jcursor(L"key 2")(a) = fostlib::json(10));
    FSL_CHECK_NOTHROW(fostlib::jcursor(L"key 2")(a) = fostlib::json(12));
    FSL_CHECK_EQ(a.size(), 2u);
    FSL_CHECK_EQ(a[L"key 2"], fostlib::json(12));
}


FSL_TEST_FUNCTION(replace) {
    fostlib::json a = fostlib::json::object_t();
    fostlib::jcursor(L"key").insert(a, L"value");
    FSL_CHECK_EQ(a.size(), 1u);
    FSL_CHECK_EQ(a[L"key"], fostlib::json(L"value"));
    FSL_CHECK_EXCEPTION(
            fostlib::jcursor(L"key 2").replace(a, 10),
            fostlib::exceptions::null &);
    fostlib::jcursor(L"key 2").insert(a, 10);
    fostlib::jcursor(L"key 2")(a) = fostlib::json(12);
    FSL_CHECK_EQ(a.size(), 2u);
    FSL_CHECK_EQ(a[L"key 2"], fostlib::json(12));
}


FSL_TEST_FUNCTION(cow) {
    fostlib::json a = fostlib::json::object_t();
    fostlib::jcursor(L"key 2")(a) = fostlib::json(L"value");
    fostlib::jcursor(L"hello")(a) = fostlib::json("goodbye");
    fostlib::json b = a;
    fostlib::jcursor(L"hello")(b) = fostlib::json(L"world");
    FSL_CHECK_EQ(a[L"hello"], fostlib::json(L"goodbye"));
    FSL_CHECK_EQ(b[L"hello"], fostlib::json(L"world"));
}
