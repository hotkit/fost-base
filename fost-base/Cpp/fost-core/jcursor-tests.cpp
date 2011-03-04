/*
    Copyright 2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/test>


FSL_TEST_SUITE(jcursor);


FSL_TEST_FUNCTION(construction_multiargument) {
    fostlib::jcursor j1("level1", "level2");
}

