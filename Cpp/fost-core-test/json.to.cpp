/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


FSL_TEST_SUITE( json_to_json );


FSL_TEST_FUNCTION( atoms ) {
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json() ), L"null" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( true ) ), L"true" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( false ) ), L"false" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( 10 ) ), L"10" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( 0 ) ), L"0" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( -10 ) ), L"-10" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( 3.141 ) ), L"3.141" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( L"Hello" ) ), L"\"Hello\"" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( L"Bob's" ) ), L"\"Bob's\"" );
    FSL_CHECK_EQ( fostlib::json::unparse( fostlib::json( L"Bob says \"Hello\"" ) ), L"\"Bob says \\\"Hello\\\"\"" );
}


FSL_TEST_FUNCTION( arrays ) {
    fostlib::json a = fostlib::json::array_t();
    FSL_CHECK_EQ( fostlib::json::unparse( a ), L"[]" );
    a.push_back( fostlib::json( 1 ) );
    a.push_back( fostlib::json( L"Hello" ) );
    FSL_CHECK_EQ( fostlib::json::unparse( a ), L"[1,\"Hello\"]" );
}


FSL_TEST_FUNCTION( objects ) {
    fostlib::json o = fostlib::json::object_t();
    FSL_CHECK_EQ( fostlib::json::unparse( o ), L"{}" );
    o.insert( L"key", L"value" );
    FSL_CHECK_EQ( fostlib::json::unparse( o ), L"{\"key\":\"value\"}" );
}


FSL_TEST_FUNCTION( tortuous ) {
    fostlib::json a = fostlib::json::array_t();
    a.push_back( fostlib::json( 1 ) );
    a.push_back( fostlib::json( L"Hello" ) );
    fostlib::json o = fostlib::json::object_t();
    o.insert( L"key", L"value" );
    o.insert( L"array", a );
    FSL_CHECK_EQ( fostlib::json::unparse( o ), L"{\"array\":[1,\"Hello\"],\"key\":\"value\"}" );
}

