 /*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


using namespace fostlib;


FSL_TEST_SUITE( json_iterator );


FSL_TEST_FUNCTION( empty ) {
    json empty;

    fostlib::test::default_copy_constructable< json::const_iterator >();

    FSL_CHECK( json::const_iterator() == empty.begin() );
}
