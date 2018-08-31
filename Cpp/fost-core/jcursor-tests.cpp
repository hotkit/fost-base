/**
    Copyright 2011-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <fost/core>
#include <fost/counter>
#include <fost/push_back>
#include <fost/test>
#include <fost/parse/jcursor.hpp>


FSL_TEST_SUITE(jcursor);


FSL_TEST_FUNCTION(construction_multiargument_string) {
    fostlib::jcursor j1("level1", "level2");
    fostlib::jcursor j2("level1", "level2", "level3");
    fostlib::jcursor j3("level1", "level2", "level3", "level4");
}

FSL_TEST_FUNCTION(construction_multiargument_numeric) {
    fostlib::jcursor j1(1, 2);
    fostlib::jcursor j2(1, 2, 3);
    fostlib::jcursor j3(1, 2, 3, 4);
}

FSL_TEST_FUNCTION(construction_initialiser_list) {
    fostlib::jcursor j1{"level1"};
    fostlib::jcursor j2{"level1", "level2"};
    fostlib::jcursor j3{"level1", "level2", "level3"};
}


FSL_TEST_FUNCTION(split) {
    FSL_CHECK_EQ(fostlib::jcursor::split("12/34", "/"), fostlib::jcursor(12, 34));
    FSL_CHECK_EQ(fostlib::jcursor::split("item.34", "."), fostlib::jcursor("item", 34));
    FSL_CHECK_EQ(fostlib::jcursor::split("item::sub", "::"), fostlib::jcursor("item", "sub"));
}


#define FC(sf, pf, jc) \
    FSL_CHECK_EQ(fostlib::jcursor::parse_json_pointer_string(sf), jc); \
    FSL_CHECK_EQ(fostlib::jcursor::parse_json_pointer_fragment(pf), jc);
FSL_TEST_FUNCTION(parse) {
    FC("", "#", fostlib::jcursor{});
    FC("/foo", "#/foo", fostlib::jcursor{"foo"});
    FC("/foo/0", "#/foo/0", fostlib::jcursor{"foo"} / 0);
    FC("/", "#/", fostlib::jcursor{""});
    FC("/a~1b", "#/a~1b", fostlib::jcursor{"a/b"});
    FC("/c%d", "#/c%25d", fostlib::jcursor{"c%d"});
    FC("/e^f", "#/e%5Ef", fostlib::jcursor{"e^f"});
    FC("/g|h", "#/g%7Ch", fostlib::jcursor{"g|h"});
    FC("/i\\\\j", "#/i%5Cj", fostlib::jcursor{"i\\j"});
    FC("/k\\\"l", "#/k%22l", fostlib::jcursor{"k\"l"});
    FC("/ ", "#/%20", fostlib::jcursor{" "});
    FC("/\\u0020", "#/%20", fostlib::jcursor{" "});
    FC("/m~0n", "#/m~0n", fostlib::jcursor{"m~n"});
}


FSL_TEST_FUNCTION(json_pointer_fragment) {
    fostlib::jcursor p;
    FSL_CHECK_EQ(p.as_json_pointer(), "");
    FSL_CHECK_EQ((p / "hello").as_json_pointer(), "/hello");
    FSL_CHECK_EQ((p / 1 / "foo").as_json_pointer(), "/1/foo");
    FSL_CHECK_EQ((p / "foo/bar").as_json_pointer(), "/foo~1bar");
    FSL_CHECK_EQ((p / "~").as_json_pointer(), "/~0");
    FSL_CHECK_EQ(
        (p / (fostlib::string("t") + 0x1d11e)).as_json_pointer(),
        "/t%F0%9D%84%9E");
}

