/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/progress.hpp>


FSL_TEST_SUITE( progress );


FSL_TEST_FUNCTION( constructor_progress ) {
    fostlib::progress parent;
    fostlib::progress child(parent);
}

