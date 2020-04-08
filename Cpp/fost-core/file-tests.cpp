/**
    Copyright 2013-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"
#include <fost/file>

#include <fost/exception/file_error.hpp>


FSL_TEST_SUITE(file);


FSL_TEST_FUNCTION(join_paths) {
    FSL_CHECK_EQ(fostlib::join_paths("", "something"), "something");
    FSL_CHECK_EQ(fostlib::join_paths("", "/something"), "/something");
    FSL_CHECK_EQ(fostlib::join_paths("path", "something"), "path/something");
    FSL_CHECK_EQ(fostlib::join_paths("path", "/something"), "/something");
    // #ifdef FOST_WINDOWS
    //     FSL_CHECK_EQ(
    //         fostlib::join_paths("", "C:\\something"), "C:\\something");
    //     FSL_CHECK_EQ(
    //         fostlib::join_paths("path", "C:\\something"), "C:\\something");
    // #endif
}


FSL_TEST_FUNCTION(unique_filename) {
    fostlib::fs::path path;
    FSL_CHECK_NOTHROW(path = fostlib::unique_filename());
    FSL_CHECK(!fostlib::fs::exists(path));
}


FSL_TEST_FUNCTION(save_file) {
    const fostlib::fs::path filename = "/nowhere/not-allowed.txt";
    FSL_CHECK_EXCEPTION(
            fostlib::utf::save_file(filename, "some text"),
            fostlib::exceptions::file_error &);
    FSL_CHECK(not fostlib::fs::exists(filename));
}
