/**
    Copyright 2008-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core-test.hpp"
#include <fost/unicode>

using namespace f5::literals;


FSL_TEST_SUITE(json_atom);


fostlib::json do_return() {
    return fostlib::json(fostlib::string(L"some string"));
}
FSL_TEST_FUNCTION(constructors) {
    fostlib::test::default_copy_constructable<fostlib::json>();
    fostlib::json v0, v1(true), v2(10), v3(L"true"), v4(L"10"), v5(L"0"),
            v6(L"Hello world!"), v7(float(1.0)), v8(double(3.141));

    fostlib::json c(fostlib::string(L"some string"));
    fostlib::json d(c);
    fostlib::json e(do_return());
    fostlib::json f("string"_l);
}


FSL_TEST_FUNCTION(isnull) {
    FSL_CHECK(fostlib::json().isnull());
    FSL_CHECK(not fostlib::json(true).isnull());
    FSL_CHECK(not fostlib::json(10).isnull());
    FSL_CHECK(not fostlib::json(L"true").isnull());
    FSL_CHECK(not fostlib::json(L"10").isnull());
    FSL_CHECK(not fostlib::json(L"0").isnull());
    FSL_CHECK(not fostlib::json(L"Hello world!").isnull());
    FSL_CHECK(not fostlib::json(double(3.14)).isnull());
    FSL_CHECK(not fostlib::json(fostlib::json::array_t()).isnull());
    FSL_CHECK(not fostlib::json(fostlib::json::object_t()).isnull());
}


FSL_TEST_FUNCTION(isatom) {
    FSL_CHECK(not fostlib::json().isatom());
    FSL_CHECK(fostlib::json(true).isatom());
    FSL_CHECK(fostlib::json(10).isatom());
    FSL_CHECK(fostlib::json(L"true").isatom());
    FSL_CHECK(fostlib::json(L"10").isatom());
    FSL_CHECK(fostlib::json(L"0").isatom());
    FSL_CHECK(fostlib::json(L"Hello world!").isatom());
    FSL_CHECK(fostlib::json(double(3.14)).isatom());
    FSL_CHECK(not fostlib::json(fostlib::json::array_t()).isatom());
    FSL_CHECK(not fostlib::json(fostlib::json::object_t()).isatom());
}


FSL_TEST_FUNCTION(isarray) {
    FSL_CHECK(not fostlib::json().isarray());
    FSL_CHECK(not fostlib::json(true).isarray());
    FSL_CHECK(not fostlib::json(10).isarray());
    FSL_CHECK(not fostlib::json(L"true").isarray());
    FSL_CHECK(not fostlib::json(L"10").isarray());
    FSL_CHECK(not fostlib::json(L"0").isarray());
    FSL_CHECK(not fostlib::json(L"Hello world!").isarray());
    FSL_CHECK(not fostlib::json(double(3.14)).isarray());
    FSL_CHECK(fostlib::json(fostlib::json::array_t()).isarray());
    FSL_CHECK(not fostlib::json(fostlib::json::object_t()).isarray());
}


FSL_TEST_FUNCTION(isobject) {
    FSL_CHECK(not fostlib::json().isobject());
    FSL_CHECK(not fostlib::json(true).isobject());
    FSL_CHECK(not fostlib::json(10).isobject());
    FSL_CHECK(not fostlib::json(L"true").isobject());
    FSL_CHECK(not fostlib::json(L"10").isobject());
    FSL_CHECK(not fostlib::json(L"0").isobject());
    FSL_CHECK(not fostlib::json(L"Hello world!").isobject());
    FSL_CHECK(not fostlib::json(double(3.14)).isobject());
    FSL_CHECK(not fostlib::json(fostlib::json::array_t()).isobject());
    FSL_CHECK(fostlib::json(fostlib::json::object_t()).isobject());
}


FSL_TEST_FUNCTION(as_int64_t) {
    fostlib::json v0, v1(true), v2(10), v3(L"true"), v4(L"10"), v5(L"0"),
            v6(L"Hello world!"), v7(float(1.0)), v8(double(3.141));

    FSL_CHECK(not v0.get<int64_t>());
    FSL_CHECK(not v1.get<int64_t>());
    FSL_CHECK_EQ(v2.get<int64_t>().value(), 10);
    FSL_CHECK_EQ(fostlib::json(0).get<int64_t>().value(), 0);
}


FSL_TEST_FUNCTION(as_string) {
    fostlib::json v0, v1(true), v2(10), v3(L"true"), v4(L"10"), v5(L"0"),
            v6(L"Hello world!"), v7(float(1.0)), v8(double(3.141)),
            v9("[Hello]");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(v1), "true");
}


FSL_TEST_FUNCTION(equality) {
    fostlib::json v0, v1(true), v2(10), v3(L"true"), v4(L"10"), v5(L"0"),
            v6(L"Hello world!"), v7(float(1.0)), v8(double(3.141));

    FSL_CHECK_EQ(v3, "true");
    FSL_CHECK_NEQ(v1, fostlib::json(false));

    FSL_CHECK_EQ(v0, v0);
    FSL_CHECK_EQ(v1, v1);
    FSL_CHECK_EQ(v2, v2);
    FSL_CHECK_EQ(v3, v3);
    FSL_CHECK_EQ(v4, v4);
    FSL_CHECK_EQ(v5, v5);
    FSL_CHECK_EQ(v6, v6);
    FSL_CHECK_EQ(v7, v7);
    FSL_CHECK_EQ(v8, v8);

    FSL_CHECK_NEQ(v0, v1);
    FSL_CHECK_NEQ(v0, v2);
    FSL_CHECK_NEQ(v0, v3);
    FSL_CHECK_NEQ(v0, v4);
    FSL_CHECK_NEQ(v0, v5);
    FSL_CHECK_NEQ(v0, v6);
    FSL_CHECK_NEQ(v0, v7);
    FSL_CHECK_NEQ(v0, v8);

    FSL_CHECK_NEQ(v1, v0);
    FSL_CHECK_NEQ(v1, v2);
    FSL_CHECK_NEQ(v1, v3);
    FSL_CHECK_NEQ(v1, v4);
    FSL_CHECK_NEQ(v1, v5);
    FSL_CHECK_NEQ(v1, v6);
    FSL_CHECK_NEQ(v1, v7);
    FSL_CHECK_NEQ(v1, v8);

    FSL_CHECK_NEQ(v2, v0);
    FSL_CHECK_NEQ(v2, v1);
    FSL_CHECK_NEQ(v2, v3);
    FSL_CHECK_NEQ(v2, v4);
    FSL_CHECK_NEQ(v2, v5);
    FSL_CHECK_NEQ(v2, v6);
    FSL_CHECK_NEQ(v2, v7);
    FSL_CHECK_NEQ(v2, v8);

    FSL_CHECK_NEQ(v3, v0);
    FSL_CHECK_NEQ(v3, v1);
    FSL_CHECK_NEQ(v3, v2);
    FSL_CHECK_NEQ(v3, v4);
    FSL_CHECK_NEQ(v3, v5);
    FSL_CHECK_NEQ(v3, v6);
    FSL_CHECK_NEQ(v3, v7);
    FSL_CHECK_NEQ(v3, v8);

    FSL_CHECK_NEQ(v4, v0);
    FSL_CHECK_NEQ(v4, v1);
    FSL_CHECK_NEQ(v4, v2);
    FSL_CHECK_NEQ(v4, v3);
    FSL_CHECK_NEQ(v4, v5);
    FSL_CHECK_NEQ(v4, v6);
    FSL_CHECK_NEQ(v4, v7);
    FSL_CHECK_NEQ(v4, v8);

    FSL_CHECK_NEQ(v5, v0);
    FSL_CHECK_NEQ(v5, v1);
    FSL_CHECK_NEQ(v5, v2);
    FSL_CHECK_NEQ(v5, v3);
    FSL_CHECK_NEQ(v5, v4);
    FSL_CHECK_NEQ(v5, v6);
    FSL_CHECK_NEQ(v5, v7);
    FSL_CHECK_NEQ(v5, v8);

    FSL_CHECK_NEQ(v6, v0);
    FSL_CHECK_NEQ(v6, v1);
    FSL_CHECK_NEQ(v6, v2);
    FSL_CHECK_NEQ(v6, v3);
    FSL_CHECK_NEQ(v6, v4);
    FSL_CHECK_NEQ(v6, v5);
    FSL_CHECK_NEQ(v6, v7);
    FSL_CHECK_NEQ(v6, v8);

    FSL_CHECK_NEQ(v7, v0);
    FSL_CHECK_NEQ(v7, v1);
    FSL_CHECK_NEQ(v7, v2);
    FSL_CHECK_NEQ(v7, v3);
    FSL_CHECK_NEQ(v7, v4);
    FSL_CHECK_NEQ(v7, v5);
    FSL_CHECK_NEQ(v7, v6);
    FSL_CHECK_NEQ(v7, v8);

    FSL_CHECK_NEQ(v8, v0);
    FSL_CHECK_NEQ(v8, v1);
    FSL_CHECK_NEQ(v8, v2);
    FSL_CHECK_NEQ(v8, v3);
    FSL_CHECK_NEQ(v8, v4);
    FSL_CHECK_NEQ(v8, v5);
    FSL_CHECK_NEQ(v8, v6);
    FSL_CHECK_NEQ(v8, v7);
}


FSL_TEST_FUNCTION(assignment) {
    fostlib::json v, v0, v1(true), v2(10), v3(L"true"), v4(L"10"), v5(L"0"),
            v6(L"Hello world!"), v7(float(1.0)), v8(double(3.141));

    v = v0;
    FSL_CHECK_EQ(v, v0);
    v = v1;
    FSL_CHECK_EQ(v, v1);
    v = v2;
    FSL_CHECK_EQ(v, v2);
    v = v3;
    FSL_CHECK_EQ(v, v3);
    v = v4;
    FSL_CHECK_EQ(v, v4);
    v = v5;
    FSL_CHECK_EQ(v, v5);
    v = v6;
    FSL_CHECK_EQ(v, v6);
    v = v7;
    FSL_CHECK_EQ(v, v7);
    v = v8;
    FSL_CHECK_EQ(v, v8);
}


FSL_TEST_FUNCTION(size) {
    fostlib::json v0, v1(true), v2(10), v3(L"true"), v4(L"10"), v5(L"0"),
            v6(L"Hello world!"), v7(float(1.0)), v8(double(3.141));

    FSL_CHECK_EQ(v0.size(), 0u);
    FSL_CHECK_EQ(v1.size(), 1u);
    FSL_CHECK_EQ(v2.size(), 1u);
    FSL_CHECK_EQ(v3.size(), 1u);
    FSL_CHECK_EQ(v4.size(), 1u);
    FSL_CHECK_EQ(v5.size(), 1u);
    FSL_CHECK_EQ(v6.size(), 1u);
    FSL_CHECK_EQ(v7.size(), 1u);
    FSL_CHECK_EQ(v8.size(), 1u);
}
