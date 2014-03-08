/*
    Copyright 2008-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/push_back>


FSL_TEST_SUITE( json_array );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::json a;

    fostlib::jcursor().push_back( a, fostlib::json() );
    FSL_CHECK( a.isarray() );
    FSL_CHECK_EQ( a[ 0 ], fostlib::json() );
}


FSL_TEST_FUNCTION( push_back ) {
    fostlib::json a;

    fostlib::jcursor().push_back( a, fostlib::json( -10 ) );
    fostlib::jcursor().push_back( a, fostlib::json( 1234 ) );
    FSL_CHECK_EQ( a[ 0 ], fostlib::json( -10 ) );
    FSL_CHECK_EQ( a[ 1 ], fostlib::json( 1234 ) );
    FSL_CHECK_EQ( a.size(), 2u );

    fostlib::json b;
    fostlib::jcursor().push_back( b, a );
    FSL_CHECK_EQ( b[ 0 ][ 0 ], fostlib::json( -10 ) );
    FSL_CHECK_EQ( b[ 0 ][ 1 ], fostlib::json( 1234 ) );
}


FSL_TEST_FUNCTION( del_key ) {
    fostlib::json a;
    fostlib::jcursor().push_back( a, fostlib::json( -10 ) );
    fostlib::jcursor().push_back( a, fostlib::json( 1234 ) );
    FSL_CHECK_EQ( a.size(), 2u );

    fostlib::jcursor(0).del_key( a );
    FSL_CHECK_EQ( a.size(), 1u );
    FSL_CHECK_EQ( a[ 0 ], fostlib::json( 1234 ) );
}


FSL_TEST_FUNCTION( access ) {
    fostlib::json a;
    fostlib::push_back(a, "key", "value1");
    fostlib::push_back(a, "key", "value2");
    FSL_CHECK_EQ(a["key"][0], fostlib::json("value1"));
    FSL_CHECK_EQ(a["key"][1], fostlib::json("value2"));
}


FSL_TEST_FUNCTION( access_out_of_bounds ) {
    fostlib::json b;
    FSL_CHECK_EXCEPTION(b[0],
        fostlib::exceptions::out_of_range_string&);
}

