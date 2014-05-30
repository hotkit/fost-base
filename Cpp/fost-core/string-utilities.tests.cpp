/*
    Copyright 2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/string>


FSL_TEST_SUITE( string_utilities );


FSL_TEST_FUNCTION(uuid) {
    fostlib::string id(fostlib::guid());
    FSL_CHECK_EQ(id[8], '-');
    FSL_CHECK_EQ(id[13], '-');
}

