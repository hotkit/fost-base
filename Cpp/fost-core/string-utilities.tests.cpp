/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"
#include <fost/string>


FSL_TEST_SUITE(string_utilities);


FSL_TEST_FUNCTION(uuid) {
    fostlib::string id(fostlib::guid());
    FSL_CHECK_EQ(id[8], '-');
    FSL_CHECK_EQ(id[13], '-');
}


FSL_TEST_FUNCTION(replace) {
    fostlib::string tstr("p1.p2.p3");
    FSL_CHECK_EQ(replace_all(tstr, ".", "-"), "p1-p2-p3");
}
