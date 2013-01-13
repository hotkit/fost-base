/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/progress.hpp>


FSL_TEST_SUITE( progress );


FSL_TEST_FUNCTION( progress_in_for_loop ) {
    const std::size_t upto = 1000;
    std::size_t sum = 0;
    for ( fostlib::progress pos(upto); !pos.is_complete(); ++pos )
        sum += pos.current();
    // Started at zero so only summed the first 999 integers
    FSL_CHECK_EQ(sum, 499500);
}

