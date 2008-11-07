/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


FSL_TEST_SUITE( json_object );


FSL_TEST_FUNCTION( constructors ) {
    FSL_CHECK( fostlib::json( fostlib::json::object_t() ).isobject() );
}


FSL_TEST_FUNCTION( insert ) {
    fostlib::json a = fostlib::json::object_t();
    a.insert( L"key", L"value" );
    FSL_CHECK_EQ( a.size(), 1 );
    FSL_CHECK_EQ( a[ L"key" ], fostlib::json( L"value" ) );
    a.insert( L"key 2", 10 );
    FSL_CHECK_EQ( a.size(), 2 );
    FSL_CHECK_EQ( a[ L"key 2" ], fostlib::json( 10 ) );
}


FSL_TEST_FUNCTION( cow ) {
    fostlib::json a = fostlib::json::object_t();
    a.insert( L"key", L"value" );
    a.insert( L"hello", "goodbye" );
    fostlib::json b = a;
    b.insert( L"hello", L"world" );
    FSL_CHECK_EQ( a[ L"hello" ], fostlib::json( L"goodbye" ) );
    FSL_CHECK_EQ( b[ L"hello" ], fostlib::json( L"world" ) );
}
