/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/push_back>


FSL_TEST_SUITE(module);


FSL_TEST_FUNCTION(builtin) {
    fostlib::json path;
    fostlib::push_back(path, "fost");
    FSL_CHECK_EQ(fostlib::c_fost.as_json(), path);
    fostlib::push_back(path, "base");
    FSL_CHECK_EQ(fostlib::c_fost_base.as_json(), path);
}


FSL_TEST_FUNCTION(dynamic) {
    fostlib::module test(fostlib::c_fost, std::to_string(1));
    fostlib::json path;
    fostlib::push_back(path, "fost");
    fostlib::push_back(path, 1);
    FSL_CHECK_EQ(test.as_json(), path);
    fostlib::module ext(test, std::string("extension/parts"));
    fostlib::push_back(path, "extension");
    fostlib::push_back(path, "parts");
    FSL_CHECK_EQ(ext.as_json(), path);
}

