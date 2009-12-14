/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"


#include <fost/exception/null.hpp>
#include <fost/exception/parse_error.hpp>


FSL_TEST_SUITE( variant_basic );


FSL_TEST_FUNCTION( constructors ) {
    fostlib::variant v0, v1( true ), v2( 10 ), v3( L"true" ), v4( L"10" ), v5( L"0" ), v6( L"Hello world!" ), v7( float( 1.0 ) ), v8( double( 3.141 ) );
}


FSL_TEST_FUNCTION( isnull ) {
    fostlib::variant v0, v1( true ), v2( 10 ), v3( L"true" ), v4( L"10" ), v5( L"Hello world!" ), v6( float( 1.0 ) ), v7( double( 3.141 ) );

    FSL_CHECK( v0.isnull() );
    FSL_CHECK( !v1.isnull() );
    FSL_CHECK( !v2.isnull() );
    FSL_CHECK( !v3.isnull() );
    FSL_CHECK( !v4.isnull() );
    FSL_CHECK( !v5.isnull() );
    FSL_CHECK( !v6.isnull() );
    FSL_CHECK( !v7.isnull() );
}


FSL_TEST_FUNCTION( as_bool ) {
    FSL_CHECK( fostlib::variant().get< bool >().isnull() );
    FSL_CHECK( fostlib::variant( true ).get< bool >().value() );

    FSL_CHECK( !fostlib::coerce< bool >( fostlib::variant() ) );
    FSL_CHECK( fostlib::coerce< bool >( fostlib::variant( 10 ) ) );
    FSL_CHECK( fostlib::coerce< bool >( fostlib::variant( L"true" ) ) );
    FSL_CHECK( fostlib::coerce< bool >( fostlib::variant( L"10" ) ) );
    FSL_CHECK( !fostlib::coerce< bool >( fostlib::variant( L"" ) ) );
    FSL_CHECK( fostlib::coerce< bool >( fostlib::variant( 3.141 ) ) );
    FSL_CHECK( !fostlib::coerce< bool >( fostlib::variant( 0. ) ) );
}


FSL_TEST_FUNCTION( as_int64_t ) {
    FSL_CHECK( fostlib::variant().get< int64_t >().isnull() );
    FSL_CHECK_EQ( fostlib::variant( 10 ).get< int64_t >().value(), 10 );

    FSL_CHECK_EXCEPTION( fostlib::coerce< int64_t >( fostlib::variant() ), fostlib::exceptions::null& );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::variant( 10 ) ), 10 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::variant( 3.141 ) ), 3 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::variant( "10" ) ), 10 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::variant( L"10" ) ), 10 );
    FSL_CHECK_EXCEPTION( fostlib::coerce< int64_t >( fostlib::variant( L"Hello 10" ) ), fostlib::exceptions::parse_error& );
    FSL_CHECK_EXCEPTION( fostlib::coerce< int64_t >( fostlib::variant( L"3.141" ) ), fostlib::exceptions::parse_error& );
}


FSL_TEST_FUNCTION( as_double ) {
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::json( L"1" ) ), 1. );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::json( L"1.5" ) ), 1.5 );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::json( L" 1" ) ), 1. );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::json( L" 1.5" ) ), 1.5 );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::json( L"1 " ) ), 1. );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::json( L"1.5 " ) ), 1.5 );
}


FSL_TEST_FUNCTION( as_wstring ) {
    FSL_CHECK_EXCEPTION( fostlib::coerce< fostlib::string >( fostlib::variant() ), fostlib::exceptions::null& );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( true ) ), L"true" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( L"10" ) ), L"10" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( L"true" ) ), L"true" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( L"10" ) ), L"10" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( 0 ) ), L"0" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( L"Hello world!" ) ), L"Hello world!" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( 1.0f ) ), fostlib::coerce< fostlib::string >( float( 1.0 ) ) );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::variant( 3.141 ) ), fostlib::coerce< fostlib::string >( double( 3.141 ) ) );
}


FSL_TEST_FUNCTION( equality ) {
    fostlib::variant v0, v1( true ), v2( 10 ), v3( L"true" ), v4( L"10" ), v5( L"0" ), v6( L"Hello world!" ), v7( float( 1.0 ) ), v8( double( 3.141 ) );

    FSL_CHECK_EQ( v0, v0 );
    FSL_CHECK_EQ( v1, v1 );
    FSL_CHECK_EQ( v2, v2 );
    FSL_CHECK_EQ( v3, v3 );
    FSL_CHECK_EQ( v4, v4 );
    FSL_CHECK_EQ( v5, v5 );
    FSL_CHECK_EQ( v6, v6 );
    FSL_CHECK_EQ( v7, v7 );
    FSL_CHECK_EQ( v8, v8 );

    FSL_CHECK_NEQ( v0, v1 );
    FSL_CHECK_NEQ( v0, v2 );
    FSL_CHECK_NEQ( v0, v3 );
    FSL_CHECK_NEQ( v0, v4 );
    FSL_CHECK_NEQ( v0, v5 );
    FSL_CHECK_NEQ( v0, v6 );
    FSL_CHECK_NEQ( v0, v7 );
    FSL_CHECK_NEQ( v0, v8 );

    FSL_CHECK_NEQ( v1, v0 );
    FSL_CHECK_NEQ( v1, v2 );
    FSL_CHECK_NEQ( v1, v3 );
    FSL_CHECK_NEQ( v1, v4 );
    FSL_CHECK_NEQ( v1, v5 );
    FSL_CHECK_NEQ( v1, v6 );
    FSL_CHECK_NEQ( v1, v7 );
    FSL_CHECK_NEQ( v1, v8 );

    FSL_CHECK_NEQ( v2, v0 );
    FSL_CHECK_NEQ( v2, v1 );
    FSL_CHECK_NEQ( v2, v3 );
    FSL_CHECK_NEQ( v2, v4 );
    FSL_CHECK_NEQ( v2, v5 );
    FSL_CHECK_NEQ( v2, v6 );
    FSL_CHECK_NEQ( v2, v7 );
    FSL_CHECK_NEQ( v2, v8 );

    FSL_CHECK_NEQ( v3, v0 );
    FSL_CHECK_NEQ( v3, v1 );
    FSL_CHECK_NEQ( v3, v2 );
    FSL_CHECK_NEQ( v3, v4 );
    FSL_CHECK_NEQ( v3, v5 );
    FSL_CHECK_NEQ( v3, v6 );
    FSL_CHECK_NEQ( v3, v7 );
    FSL_CHECK_NEQ( v3, v8 );

    FSL_CHECK_NEQ( v4, v0 );
    FSL_CHECK_NEQ( v4, v1 );
    FSL_CHECK_NEQ( v4, v2 );
    FSL_CHECK_NEQ( v4, v3 );
    FSL_CHECK_NEQ( v4, v5 );
    FSL_CHECK_NEQ( v4, v6 );
    FSL_CHECK_NEQ( v4, v7 );
    FSL_CHECK_NEQ( v4, v8 );

    FSL_CHECK_NEQ( v5, v0 );
    FSL_CHECK_NEQ( v5, v1 );
    FSL_CHECK_NEQ( v5, v2 );
    FSL_CHECK_NEQ( v5, v3 );
    FSL_CHECK_NEQ( v5, v4 );
    FSL_CHECK_NEQ( v5, v6 );
    FSL_CHECK_NEQ( v5, v7 );
    FSL_CHECK_NEQ( v5, v8 );

    FSL_CHECK_NEQ( v6, v0 );
    FSL_CHECK_NEQ( v6, v1 );
    FSL_CHECK_NEQ( v6, v2 );
    FSL_CHECK_NEQ( v6, v3 );
    FSL_CHECK_NEQ( v6, v4 );
    FSL_CHECK_NEQ( v6, v5 );
    FSL_CHECK_NEQ( v6, v7 );
    FSL_CHECK_NEQ( v6, v8 );

    FSL_CHECK_NEQ( v7, v0 );
    FSL_CHECK_NEQ( v7, v1 );
    FSL_CHECK_NEQ( v7, v2 );
    FSL_CHECK_NEQ( v7, v3 );
    FSL_CHECK_NEQ( v7, v4 );
    FSL_CHECK_NEQ( v7, v5 );
    FSL_CHECK_NEQ( v7, v6 );
    FSL_CHECK_NEQ( v7, v8 );

    FSL_CHECK_NEQ( v8, v0 );
    FSL_CHECK_NEQ( v8, v1 );
    FSL_CHECK_NEQ( v8, v2 );
    FSL_CHECK_NEQ( v8, v3 );
    FSL_CHECK_NEQ( v8, v4 );
    FSL_CHECK_NEQ( v8, v5 );
    FSL_CHECK_NEQ( v8, v6 );
    FSL_CHECK_NEQ( v8, v7 );
}


#ifdef FOST_USE_LONG
FSL_TEST_FUNCTION( equality_long ) {
    fostlib::variant v0, v1( true ), v2( 10 ), v3( L"true" ), v4( L"10" ), v5( L"0" ), v6( L"Hello world!" ), v7( float( 1.0 ) ), v8( double( 3.141 ) );
    fostlib::variant v9( 10UL );

    FSL_CHECK_EQ( v2, v9 );
}
#endif


FSL_TEST_FUNCTION( assignment ) {
    fostlib::variant v, v0, v1( true ), v2( 10 ), v3( L"true" ), v4( L"10" ), v5( L"0" ), v6( L"Hello world!" ), v7( float( 1.0 ) ), v8( double( 3.141 ) );

    v = v0; FSL_CHECK_EQ( v, v0 );
    v = v1; FSL_CHECK_EQ( v, v1 );
    v = v2; FSL_CHECK_EQ( v, v2 );
    v = v3; FSL_CHECK_EQ( v, v3 );
    v = v4; FSL_CHECK_EQ( v, v4 );
    v = v5; FSL_CHECK_EQ( v, v5 );
    v = v6; FSL_CHECK_EQ( v, v6 );
    v = v7; FSL_CHECK_EQ( v, v7 );
    v = v8; FSL_CHECK_EQ( v, v8 );
}

