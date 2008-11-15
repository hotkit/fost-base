/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet-test.hpp"
#include <fost/url.hpp>
#include <fost/parse/url.hpp>


using namespace fostlib;


FSL_TEST_SUITE( internet_url );


FSL_TEST_FUNCTION( query_string ) {
    url::query_string q1, q2;
    FSL_CHECK_EQ( q1.as_string().value( string() ), q2.as_string().value( string() ) );
    q1 = q2;
    q1.append( L"key", null );
    FSL_CHECK_EQ( q1.as_string().value(), L"key=" );
    q1.append( L"key", null );
    FSL_CHECK_EQ( q1.as_string().value(), L"key=&key=" );
    q2 = q1;
    FSL_CHECK_EQ( q2.as_string().value(), L"key=&key=" );
}

#define QS_PARSE( str ) \
    FSL_CHECK( boost::spirit::parse( ( str ), query_string_p[ phoenix::var( qs ) = phoenix::arg1 ] ).full ); \
    FSL_CHECK_EQ( qs.as_string().value(), (str) );
FSL_TEST_FUNCTION( query_string_parser ) {
    url::query_string qs;
    FSL_CHECK( boost::spirit::parse( L"", query_string_p[ phoenix::var( qs ) = phoenix::arg1 ] ).full );
    FSL_CHECK( qs.as_string().isnull() );
    QS_PARSE( L"key=value&key=value" );
    QS_PARSE( L"key=value" );
    QS_PARSE( L"key=" );
    QS_PARSE( L"key=&key=" );
    QS_PARSE( L"key=value&key=" );
}

/*
FSL_TEST_FUNCTION( path_spec ) {
    url u( L"http://localhost/" );
    u.pathspec( L"/file-name" );
    FSL_CHECK_EQ( u.as_string(), L"http://localhost/file-name" );
}


FSL_TEST_FUNCTION( parse ) {
    url a( L"http://localhost/" );
    FSL_CHECK_EQ( a.server().name(), L"localhost" );
    FSL_CHECK( a.user().isnull() );
}

FSL_TEST_FUNCTION( parse_port ) {
    url  a( L"http://localhost:8000/" );
    FSL_CHECK_EQ( a.server().name(), L"localhost" );
    FSL_CHECK( a.user().isnull() );
    FSL_CHECK_EQ( a.server().service(), L"8000" );
}

FSL_TEST_FUNCTION( parse_credentials ) {
    url  b( L"http://user:pass@localhost/" );
    FSL_CHECK_EQ( b.server().name(), L"localhost" );
    FSL_CHECK( !b.user().isnull() );
    FSL_CHECK_EQ( b.user().value(), L"user" );
    FSL_CHECK_EQ( b.password().value(), L"pass" );
}

FSL_TEST_FUNCTION( parse_credentials_port ) {
    url  c( L"http://user:pass@localhost:8000/" );
    FSL_CHECK_EQ( c.server().name(), L"localhost" );
    FSL_CHECK( !c.user().isnull() );
    FSL_CHECK_EQ( c.user().value(), L"user" );
    FSL_CHECK_EQ( c.password().value(), L"pass" );
    FSL_CHECK_EQ( c.server().service(), L"8000" );
}
*/