/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


#include <fost/log>
#include <fost/exception/parse_error.hpp>


FSL_TEST_SUITE( yaml );


FSL_TEST_FUNCTION( fields ) {
    fostlib::yaml::record r;

    FSL_CHECK_NOTHROW( r.add("f1").add("f2", fostlib::null) );
}

