/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


#include <fost/push_back.hpp>


FSL_TEST_SUITE( push_back );


namespace {
    template< typename C >
    C check_nullary() {
        C v;
        FSL_CHECK_NOTHROW( fostlib::push_back(v, 0) );
        FSL_CHECK_NOTHROW( fostlib::push_back(v, 1) );
        FSL_CHECK_EQ(v.size(), 2u);
        FSL_CHECK_EQ(*v.begin(), 0);
        FSL_CHECK_EQ(*(++v.begin()), 1);
        return v;
    }
}
FSL_TEST_FUNCTION( nullary ) {
    check_nullary< std::vector< int > >();
    check_nullary< std::list< int > >();
}

