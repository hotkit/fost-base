/*
    Copyright 2013-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/file>


FSL_TEST_SUITE(file);


FSL_TEST_FUNCTION(unique_filename) {
    boost::filesystem::wpath path;
    FSL_CHECK_NOTHROW(path = fostlib::unique_filename());
    FSL_CHECK(!boost::filesystem::exists(path));
}


FSL_TEST_FUNCTION(join_paths) {
    FSL_CHECK_EQ(
        fostlib::join_paths("", "something"), "something");
    FSL_CHECK_EQ(
        fostlib::join_paths("", "/something"), "/something");
    FSL_CHECK_EQ(
        fostlib::join_paths("path", "something"), "path/something");
    FSL_CHECK_EQ(
        fostlib::join_paths("path", "/something"), "/something");
// #ifdef FOST_WINDOWS
//     FSL_CHECK_EQ(
//         fostlib::join_paths("", "C:\\something"), "C:\\something");
//     FSL_CHECK_EQ(
//         fostlib::join_paths("path", "C:\\something"), "C:\\something");
// #endif
}

