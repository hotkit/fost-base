/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

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
    FSL_CHECK_EQ(a["nokey"], fostlib::json());
    fostlib::jcursor("key").insert(a, fostlib::json("value"));
    FSL_CHECK_EQ(a["key"], fostlib::json("value"));

    FSL_CHECK_EXCEPTION(
            fostlib::json()["nokey"], fostlib::exceptions::json_error &);
    fostlib::json array = fostlib::json::array_t();
    FSL_CHECK_EXCEPTION(array["nokey"], fostlib::exceptions::json_error &);

    fostlib::string key("\u0079");
    FSL_CHECK(not a.has_key(key));
    FSL_CHECK_EQ(a[key], fostlib::json());
}


FSL_TEST_FUNCTION(insert) {
    fostlib::json a = fostlib::json::object_t();
    fostlib::jcursor("key").insert(a, fostlib::json("value"));
    FSL_CHECK_EQ(a.size(), 1u);
    FSL_CHECK_EQ(a["key"], fostlib::json("value"));
    fostlib::jcursor("key 2").insert(a, fostlib::json(10));
    FSL_CHECK_EXCEPTION(
            fostlib::jcursor("key 2").insert(a, fostlib::json(12)),
            fostlib::exceptions::not_null &);
    FSL_CHECK_EQ(a.size(), 2u);
    FSL_CHECK_EQ(a["key 2"], fostlib::json(10));
}


FSL_TEST_FUNCTION(set) {
    fostlib::json a = fostlib::json::object_t();
    FSL_CHECK_NOTHROW(fostlib::jcursor("key")(a) = fostlib::json("value"));
    FSL_CHECK_EQ(a.size(), 1u);
    FSL_CHECK_EQ(a["key"], fostlib::json("value"));
    FSL_CHECK_NOTHROW(fostlib::jcursor("key 2")(a) = fostlib::json(10));
    FSL_CHECK_NOTHROW(fostlib::jcursor("key 2")(a) = fostlib::json(12));
    FSL_CHECK_EQ(a.size(), 2u);
    FSL_CHECK_EQ(a["key 2"], fostlib::json(12));
}


FSL_TEST_FUNCTION(replace) {
    fostlib::json a = fostlib::json::object_t();
    fostlib::jcursor("key").insert(a, "value");
    FSL_CHECK_EQ(a.size(), 1u);
    FSL_CHECK_EQ(a["key"], fostlib::json("value"));
    FSL_CHECK_EXCEPTION(
            fostlib::jcursor("key 2").replace(a, 10),
            fostlib::exceptions::null &);
    fostlib::jcursor("key 2").insert(a, 10);
    fostlib::jcursor("key 2")(a) = fostlib::json(12);
    FSL_CHECK_EQ(a.size(), 2u);
    FSL_CHECK_EQ(a["key 2"], fostlib::json(12));
}


FSL_TEST_FUNCTION(cow) {
    fostlib::json a = fostlib::json::object_t();
    fostlib::jcursor("key 2")(a) = fostlib::json("value");
    fostlib::jcursor("hello")(a) = fostlib::json("goodbye");
    fostlib::json b = a;
    fostlib::jcursor("hello")(b) = fostlib::json("world");
    FSL_CHECK_EQ(a["hello"], fostlib::json("goodbye"));
    FSL_CHECK_EQ(b["hello"], fostlib::json("world"));
}
