/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/memoize>


FSL_TEST_SUITE( memoize );


namespace {
    int64_t p_1() {
        return 1;
    }
}


FSL_TEST_FUNCTION( construct ) {
    fostlib::memoize< int64_t > p( boost::lambda::bind( p_1 ) );
    FSL_CHECK_EQ( p(), 1 );
}
