/**
    Copyright 2012-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core-test.hpp"

#include <fost/dynlib>


namespace {
    const fostlib::setting<fostlib::string> c_loadlib(
            "dynlib.cpp", "dynlib tests", "Load library", "fost-datetime", true);
}


FSL_TEST_SUITE(dynlib);


FSL_TEST_FUNCTION(datetime) {
    FSL_CHECK_NOTHROW(fostlib::dynlib(c_loadlib.value()));
}
