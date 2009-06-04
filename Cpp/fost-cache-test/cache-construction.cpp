/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cache-test.hpp"
#include <fost/cache>


FSL_TEST_SUITE( construction );

FSL_TEST_FUNCTION( objectcache ) {
    fostlib::objectcache< fostlib::meta_instance > cache;
}


FSL_TEST_FUNCTION( fostcache ) {
    fostlib::fostcache cache;
}
