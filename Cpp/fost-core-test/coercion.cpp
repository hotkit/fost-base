/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"

#include <fost/exception/not_a_number.hpp>
#include <fost/exception/parse_error.hpp>


FSL_TEST_SUITE( coercion );


FSL_TEST_FUNCTION( int64_t ) {
    FSL_CHECK_EXCEPTION( fostlib::coerce< int64_t >( fostlib::string() ), fostlib::exceptions::parse_error& );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"0" ) ), 0 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"123" ) ), 123 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"1234" ) ), 1234 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"12345" ) ), 12345 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"123456" ) ), 123456 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"1234567" ) ), 1234567 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"17179869184" ) ), int64_t( 1 ) << 34 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"18014398509481984" ) ), int64_t( 1 ) << 54 );
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"4611686018427387904" ) ), int64_t( 1 ) << 62 );;
    FSL_CHECK_EQ( fostlib::coerce< int64_t >( fostlib::string( L"-123" ) ), -123 );
}


FSL_TEST_FUNCTION( uint16_t ) {
    FSL_CHECK_EXCEPTION( fostlib::coerce< uint16_t >( fostlib::string() ), fostlib::exceptions::parse_error& );
    FSL_CHECK_EQ( fostlib::coerce< uint16_t >( fostlib::string( L"0" ) ), 0 );
    FSL_CHECK_EQ( fostlib::coerce< uint16_t >( fostlib::string( L"123" ) ), 123 );
    FSL_CHECK_EQ( fostlib::coerce< uint16_t >( fostlib::string( L"1234" ) ), 1234 );
    FSL_CHECK_EQ( fostlib::coerce< uint16_t >( fostlib::string( L"12345" ) ), 12345 );
    FSL_CHECK_EXCEPTION( fostlib::coerce< uint16_t >( fostlib::string( L"123456" ) ), fostlib::exceptions::parse_error& );
    FSL_CHECK_EXCEPTION( fostlib::coerce< uint16_t >( fostlib::string( L"-123" ) ), fostlib::exceptions::parse_error& );
}


FSL_TEST_FUNCTION( double ) {
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::string( L"1" ) ), 1. );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::string( L"1.5" ) ), 1.5 );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::string( L" 1" ) ), 1. );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::string( L" 1.5" ) ), 1.5 );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::string( L"1 " ) ), 1. );
    FSL_CHECK_EQ( fostlib::coerce< double >( fostlib::string( L"1.5 " ) ), 1.5 );
}


FSL_TEST_FUNCTION( string ) {
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::null ), fostlib::string() );

    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( 10 ), L"10" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( 3.141 ), L"3.141" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( 3.141f ), L"3.1410000324249268" );

    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( "3.141f" ), L"3.141f" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( L"Hello world" ), L"Hello world" );

    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( 1 << 14 ), L"16384" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( 1 << 24 ), L"16777216" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( 1 ) << 34 ), L"17179869184" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( 1 ) << 44 ), L"17592186044416" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( 1 ) << 54 ), L"18014398509481984" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( 1 ) << 62 ), L"4611686018427387904" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( -1 << 14 ), L"-16384" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( -1 << 24 ), L"-16777216" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( -1 ) << 34 ), L"-17179869184" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( -1 ) << 44 ), L"-17592186044416" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( -1 ) << 54 ), L"-18014398509481984" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( int64_t( -1 ) << 62 ), L"-4611686018427387904" );

    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( uint64_t( 1 ) << 34 ), L"17179869184" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( uint64_t( 1 ) << 44 ), L"17592186044416" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( uint64_t( 1 ) << 54 ), L"18014398509481984" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( uint64_t( 1 ) << 63 ), L"9223372036854775808" );
}
