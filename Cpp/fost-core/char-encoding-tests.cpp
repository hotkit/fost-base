/*
    Copyright 2009-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/unicode>

#include <fost/exception/unicode_encoding.hpp>


using namespace fostlib::utf;


FSL_TEST_SUITE( char_encoding );


FSL_TEST_FUNCTION( lengths_narrow ) {
    FSL_CHECK_EQ( length( "" ), 0u );
    FSL_CHECK_EQ( length( "a" ), 1u );
}

FSL_TEST_FUNCTION( lengths_utf8 ) {
    FSL_CHECK_EQ( utf8length( 0x20 ), 1u ); // space
    FSL_CHECK_EQ( utf8length( 0xe5 ), 2u ); // latin small letter a with ring above
    FSL_CHECK_EQ( utf8length( 0x3b3 ), 2u ); // greek small letter gamma
    FSL_CHECK_EQ( utf8length( 0x2014 ), 3u ); // em dash
    FSL_CHECK_EQ( utf8length( 0xffe1 ), 3u ); // full width pound sign
    FSL_CHECK_EQ( utf8length( 0x1d11e ), 4u ); // treble clef
}

FSL_TEST_FUNCTION( lengths_utf16 ) {
    FSL_CHECK_EQ( utf16length( 0x20 ), 1u ); // space
    FSL_CHECK_EQ( utf16length( 0xe5 ), 1u ); // latin small letter a with ring above
    FSL_CHECK_EQ( utf16length( 0x3b3 ), 1u ); // greek small letter gamma
    FSL_CHECK_EQ( utf16length( 0x2014 ), 1u ); // em dash
    FSL_CHECK_EQ( utf16length( 0xffe1 ), 1u ); // full width pound sign
    FSL_CHECK_EQ( utf16length( 0x1d11e ), 2u ); // treble clef
}


FSL_TEST_FUNCTION( decode_wchar_t ) {
    FSL_CHECK_EQ( decode( 0x20 ), 0x20 );
    FSL_CHECK_EQ( decode( 0xe5 ), 0xe5 );
    FSL_CHECK_EQ( decode( 0x2014 ), 0x2014 );
    FSL_CHECK_EQ( decode( 0xffe1 ), 0xffe1 );
    FSL_CHECK_EQ( decode( 0xd834, 0xdd1e ), 0x1d11e );
    if ( sizeof( wchar_t ) > 2 )
        FSL_CHECK_EXCEPTION( decode( wchar_t( 0x1d11e ) ), fostlib::exceptions::unicode_encoding& );
}


FSL_TEST_FUNCTION( coerce ) {
    char s[] = { '\xc3', '\xa6', '\x00' };
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::ascii_string( "abc" ) ), L"abc" );

    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::utf8_string( "abc" ) ), L"abc" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::utf8_string( s ) ), L"\xe6" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >(
        fostlib::utf8_string("S") + fostlib::utf8_string( s ) + fostlib::utf8_string("lensminde")
    ), L"S\xe6lensminde" );
}
