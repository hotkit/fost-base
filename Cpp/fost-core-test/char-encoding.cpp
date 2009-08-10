/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/unicode.hpp>
#include <fost/exception/unicode_encoding.hpp>
#include <fost/detail/coerce.hpp>


using namespace fostlib::utf;


FSL_TEST_SUITE( char_encoding );


FSL_TEST_FUNCTION( lengths_narrow ) {
    FSL_CHECK_EQ( length( "" ), 0 );
    FSL_CHECK_EQ( length( "a" ), 1 );
}

FSL_TEST_FUNCTION( lengths_utf8 ) {
    FSL_CHECK_EQ( utf8length( 0x20 ), 1 ); // space
    FSL_CHECK_EQ( utf8length( 0xe5 ), 2 ); // latin small letter a with ring above
    FSL_CHECK_EQ( utf8length( 0x3b3 ), 2 ); // greek small letter gamma
    FSL_CHECK_EQ( utf8length( 0x2014 ), 3 ); // em dash
    FSL_CHECK_EQ( utf8length( 0xffe1 ), 3 ); // full width pound sign
    FSL_CHECK_EQ( utf8length( 0x1d11e ), 4 ); // treble clef
}

FSL_TEST_FUNCTION( lengths_utf16 ) {
    FSL_CHECK_EQ( utf16length( 0x20 ), 1 ); // space
    FSL_CHECK_EQ( utf16length( 0xe5 ), 1 ); // latin small letter a with ring above
    FSL_CHECK_EQ( utf16length( 0x3b3 ), 1 ); // greek small letter gamma
    FSL_CHECK_EQ( utf16length( 0x2014 ), 1 ); // em dash
    FSL_CHECK_EQ( utf16length( 0xffe1 ), 1 ); // full width pound sign
    FSL_CHECK_EQ( utf16length( 0x1d11e ), 2 ); // treble clef
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
    char s[] = { 0xc3, 0xa6, 0x00 };
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( std::string( "abc" ) ), L"abc" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( std::string( s ) ), L"\xe6" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( "S" + std::string( s ) + "lensminde" ), L"S\xe6lensminde" );

    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::utf8string( "abc" ) ), L"abc" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( fostlib::utf8string( s ) ), L"\xe6" );
    FSL_CHECK_EQ( fostlib::coerce< fostlib::string >( "S" + fostlib::utf8string( s ) + "lensminde" ), L"S\xe6lensminde" );
}
