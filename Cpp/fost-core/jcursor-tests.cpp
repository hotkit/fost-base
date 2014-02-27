/*
    Copyright 2011-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/push_back>
#include <fost/test>


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
