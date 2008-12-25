/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-jsondb-test.hpp"
#include <fost/jsondb>


using namespace fostlib;


FSL_TEST_SUITE( blob );


FSL_TEST_FUNCTION( construct ) {
    FSL_CHECK_NOTHROW( jsondb database );
    FSL_CHECK_NOTHROW( jsondb database; jsondb::local loc( database ) );
}
