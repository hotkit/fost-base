/**
    Copyright 2009-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core-test.hpp"
#include <fost/unicode>


FSL_TEST_SUITE(boost_filesystem_path);


FSL_TEST_FUNCTION(coercion_string) {
    fostlib::fs::path p(L"somefilepath");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::string>(p), L"somefilepath");
}


FSL_TEST_FUNCTION(coercion_json) {
    fostlib::fs::path p(L"somefilepath");
    fostlib::json j(L"somefilepath");
    FSL_CHECK_EQ(fostlib::coerce<fostlib::json>(p), j);
    FSL_CHECK_EQ(fostlib::coerce<fostlib::fs::path>(j), p);
}
