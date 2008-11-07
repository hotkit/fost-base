/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


FSL_TEST_SUITE( json_array );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::json a;

    a.push_back( fostlib::json() );
    FSL_CHECK( a.isarray() );
    FSL_CHECK_EQ( a[ 0 ], fostlib::json() );
}


FSL_TEST_FUNCTION( push_back ) {
    fostlib::json a;

    a.push_back( fostlib::json( -10 ) );
    a.push_back( fostlib::json( 1234 ) );
    FSL_CHECK_EQ( a[ 0 ], fostlib::json( -10 ) );
    FSL_CHECK_EQ( a[ 1 ], fostlib::json( 1234 ) );
    FSL_CHECK_EQ( a.size(), 2 );

    fostlib::json b;
    b.push_back( a );
    FSL_CHECK_EQ( b[ 0 ][ 0 ], fostlib::json( -10 ) );
    FSL_CHECK_EQ( b[ 0 ][ 1 ], fostlib::json( 1234 ) );
}

