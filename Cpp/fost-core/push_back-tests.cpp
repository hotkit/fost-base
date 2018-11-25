/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


#include <fost/push_back.hpp>


FSL_TEST_SUITE(push_back);


namespace {
    template<typename C>
    C check_nullary() {
        C v;
        FSL_CHECK_NOTHROW(fostlib::push_back(v, 0));
        FSL_CHECK_NOTHROW(fostlib::push_back(v, 1));
        FSL_CHECK_EQ(v.size(), 2u);
        FSL_CHECK_EQ(*v.begin(), 0);
        FSL_CHECK_EQ(*(++v.begin()), 1);
        return v;
    }
}
FSL_TEST_FUNCTION(nullary) {
    check_nullary<std::vector<int>>();
    check_nullary<std::list<int>>();
}


FSL_TEST_FUNCTION(long_chain) {
    fostlib::json j;
    fostlib::push_back(j, "one1", "two", "three", "four", "five", true);
    FSL_CHECK_EQ(
            j["one1"]["two"]["three"]["four"]["five"][0], fostlib::json(true));
    fostlib::push_back(j, "one2", "two", "three", "four", "five", "six", true);
    FSL_CHECK_EQ(
            j["one2"]["two"]["three"]["four"]["five"]["six"][0],
            fostlib::json(true));
}
