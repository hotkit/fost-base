/*
    Copyright 2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/string>


FSL_TEST_SUITE(nullable);


FSL_TEST_FUNCTION(comparison) {
    fostlib::nullable<int> ni1, ni2, i3{3};
    FSL_CHECK_EQ(ni1, ni2);
    FSL_CHECK_NEQ(ni1, i3);

    fostlib::nullable<fostlib::string> ns1, ns2, s3{"hi"};
    FSL_CHECK_EQ(ns1, ns2);
    FSL_CHECK_NEQ(ns1, s3);
}

