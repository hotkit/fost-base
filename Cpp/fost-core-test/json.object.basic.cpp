/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/exception/not_null.hpp>


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
    FSL_CHECK_EXCEPTION( a.insert( L"key 2", 12 ), fostlib::exceptions::not_null& );
    FSL_CHECK_EQ( a.size(), 2 );
    FSL_CHECK_EQ( a[ L"key 2" ], fostlib::json( 10 ) );
}


FSL_TEST_FUNCTION( set ) {
    fostlib::json a = fostlib::json::object_t();
    a.set( L"key", L"value" );
    FSL_CHECK_EQ( a.size(), 1 );
    FSL_CHECK_EQ( a[ L"key" ], fostlib::json( L"value" ) );
    a.set( L"key 2", 10 );
    a.set( L"key 2", 12 );
    FSL_CHECK_EQ( a.size(), 2 );
    FSL_CHECK_EQ( a[ L"key 2" ], fostlib::json( 12 ) );
}


FSL_TEST_FUNCTION( replace ) {
    fostlib::json a = fostlib::json::object_t();
    a.insert( L"key", L"value" );
    FSL_CHECK_EQ( a.size(), 1 );
    FSL_CHECK_EQ( a[ L"key" ], fostlib::json( L"value" ) );
    FSL_CHECK_EXCEPTION( a.replace( L"key 2", 10 ), fostlib::exceptions::null& );
    a.insert( L"key 2", 10 );
    a.replace( L"key 2", 12 );
    FSL_CHECK_EQ( a.size(), 2 );
    FSL_CHECK_EQ( a[ L"key 2" ], fostlib::json( 12 ) );
}


FSL_TEST_FUNCTION( cow ) {
    fostlib::json a = fostlib::json::object_t();
    a.set( L"key", L"value" );
    a.set( L"hello", "goodbye" );
    fostlib::json b = a;
    b.set( L"hello", L"world" );
    FSL_CHECK_EQ( a[ L"hello" ], fostlib::json( L"goodbye" ) );
    FSL_CHECK_EQ( b[ L"hello" ], fostlib::json( L"world" ) );
}
