/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/exception/json_error.hpp>
#include <fost/exception/null.hpp>


FSL_TEST_SUITE( json_jcursor );


FSL_TEST_FUNCTION( empty ) {
    fostlib::test::default_copy_constructable< fostlib::jcursor >();
    FSL_CHECK_EQ( ( fostlib::json() )[ fostlib::jcursor() ], fostlib::json() );
    FSL_CHECK_EQ( fostlib::json( 10 )[ fostlib::jcursor() ], fostlib::json( 10 ) );
    FSL_CHECK_EQ( fostlib::json( L"Hello" )[ fostlib::jcursor() ], fostlib::json( L"Hello" ) );
    FSL_CHECK_EXCEPTION( ++fostlib::jcursor(), fostlib::exceptions::null& );
}


FSL_TEST_FUNCTION( equality ) {
    fostlib::jcursor jc;
    FSL_CHECK( fostlib::jcursor() == fostlib::jcursor() );
    FSL_CHECK( jc / L"path" != fostlib::jcursor() );
    FSL_CHECK( jc / 3 != jc / L"3" );
    FSL_CHECK( jc / L"path" == fostlib::jcursor( L"path" ) );
    FSL_CHECK( jc / 3 == fostlib::jcursor( 3 ) );
}


FSL_TEST_FUNCTION( non_empty ) {
    fostlib::json a = fostlib::json( fostlib::json::object_t() );
    fostlib::jcursor( L"key" )( a ) = fostlib::json( 10 );

    FSL_CHECK_EQ( a[ fostlib::jcursor() ], a );
    FSL_CHECK_EQ( a[ fostlib::jcursor( L"key" ) ], fostlib::json( 10 ) );

    fostlib::json b = fostlib::json( fostlib::json::array_t() );
    fostlib::jcursor( 0 )( b ) = fostlib::json( 10 );
    fostlib::jcursor( 1 )( b ) = fostlib::json( 20 );

    FSL_CHECK_EQ( b[ fostlib::jcursor() ], b );
    FSL_CHECK_EQ( b[ fostlib::jcursor( 0 ) ], fostlib::json( 10 ) );
    FSL_CHECK_EQ( b[ fostlib::jcursor( 1 ) ], fostlib::json( 20 ) );
}


FSL_TEST_FUNCTION( json_position ) {
    fostlib::json a = fostlib::json( fostlib::json::object_t() );
    fostlib::jcursor( L"key" )( a ) = fostlib::json( 123 );
    fostlib::json b = fostlib::json( fostlib::json::array_t() );
    fostlib::jcursor( 0 )( b ) = fostlib::json( 10 );
    fostlib::jcursor( 1 )( b ) = fostlib::json( 20 );
    fostlib::jcursor( 2 )( b ) = a;
    FSL_CHECK_EQ( a[ fostlib::jcursor( fostlib::json( L"key" ) ) ], fostlib::json( 123 ) );
    FSL_CHECK_EQ( b[ fostlib::jcursor( fostlib::json( 0 ) ) ], fostlib::json( 10 ) );
    FSL_CHECK_EQ( b[ fostlib::jcursor( fostlib::json( 1 ) ) ], fostlib::json( 20 ) );
    FSL_CHECK_EQ( b[ fostlib::jcursor( fostlib::json( 2 ) ) ], a );
}


FSL_TEST_FUNCTION( updates ) {
    fostlib::json j;
    FSL_CHECK_NOTHROW( fostlib::jcursor( L"key" )( j ) = fostlib::json( L"value" ) );
    FSL_CHECK_EQ( j[ L"key" ], fostlib::json( L"value" ) );
    FSL_CHECK_EXCEPTION( ( fostlib::jcursor( L"key" ) / L"1st" )( j ), fostlib::exceptions::json_error& );
    FSL_CHECK_NOTHROW( ( fostlib::jcursor( L"key2" ) / L"1st" )( j ) = fostlib::json( L"value 1" ) );
    FSL_CHECK_NOTHROW( ( fostlib::jcursor( L"key2" ) / L"2nd" )( j ) = fostlib::json( L"value 2" ) );
    FSL_CHECK_EQ( j[ L"key" ], fostlib::json( L"value" ) );
    FSL_CHECK_EQ( j[ L"key2" ][ L"1st" ], fostlib::json( L"value 1" ) );
    FSL_CHECK_EQ( j[ L"key2" ][ L"2nd" ], fostlib::json( L"value 2" ) );
    FSL_CHECK_NOTHROW( ( fostlib::jcursor( L"key3" ) / 0 )( j ) = fostlib::json( L"value x" ) );
    FSL_CHECK_NOTHROW( ( fostlib::jcursor( L"key3" ) / 1 )( j ) = fostlib::json( L"value y" ) );
    FSL_CHECK_EQ( j[ L"key" ], fostlib::json( L"value" ) );
    FSL_CHECK_EQ( j[ L"key2" ][ L"1st" ], fostlib::json( L"value 1" ) );
    FSL_CHECK_EQ( j[ L"key2" ][ L"2nd" ], fostlib::json( L"value 2" ) );
    FSL_CHECK_EQ( j[ L"key3" ][ 0 ], fostlib::json( L"value x" ) );
    FSL_CHECK_EQ( j[ L"key3" ][ 1 ], fostlib::json( L"value y" ) );
}


FSL_TEST_FUNCTION( tortuous ) {
    fostlib::json j( fostlib::json::parse(
        L"[\
            10,\
            20,\
            [ true, false ],\
            {\
                \"id\": 1234,\
                \"type\": \"FSLib::Type\"\
            },\
            { \"latitude\": 6.234, \"longitude\": 53.12353 }\
        ]"
    ) );

    fostlib::jcursor p;
    FSL_CHECK_EQ( j[ p ], j );
    FSL_CHECK_NOTHROW( p = p / 0 );
    FSL_CHECK_EQ( j[ p ], fostlib::json( 10 ) );
    FSL_CHECK_NOTHROW( ++p );
    FSL_CHECK_EQ( j[ p ], fostlib::json( 20 ) );
    FSL_CHECK( j[ ++p ].isarray() );
    FSL_CHECK_EQ( j[ p.enter() ], fostlib::json( true ) );
    FSL_CHECK_EQ( j[ ++p ], fostlib::json( false ) );
    FSL_CHECK_EQ( j[ fostlib::jcursor( 2 ) / 1 ], fostlib::json( false ) );
    FSL_CHECK_EQ( j[ p.pop() ], j[ 2 ] );
    FSL_CHECK_EQ( j[ ++p ], j[ 3 ] );
    FSL_CHECK_EQ( j[ p.enter( L"id" ) ], fostlib::json( 1234 ) );
    FSL_CHECK_NOTHROW( p.pop(); ++p; );
    FSL_CHECK_EQ( j[ p ][ L"latitude" ], fostlib::json( 6.234 ) );
    FSL_CHECK_EQ( j[ p ][ L"longitude" ], fostlib::json( 53.12353 ) );
}

