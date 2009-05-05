/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/string/tagged-string.hpp>

#include <fost/exception/parse_error.hpp>


FSL_TEST_SUITE( tagged_string );

struct tag_type {
    static void do_encode( fostlib::wliteral from, fostlib::string &into ) {
        into.clear();
        for ( wchar_t c( *from ); c; c = *++from ) {
            into += c; into += L' ';
        }
    }
    static void do_encode( const fostlib::string &from, fostlib::string &into ) {
        into.clear(); into.reserve( from.length() * 2 );
        for ( fostlib::string::const_iterator c( from.begin() ); c != from.end(); ++c ) {
            into += *c; into += L' ';
        }
    }
    static void check_encoded( const fostlib::string &s ) {
        if ( ( s.length() & 1 ) != 0 )
            throw fostlib::exceptions::parse_error( L"Encoded string must be an even number of characters long" );
        bool check = false;
        for ( fostlib::string::const_iterator c( s.begin() ); c != s.end(); ++c, check = !check )
            if ( check && *c != L' ' )
                throw fostlib::exceptions::parse_error( L"Non space found at even position" );
    }
};
typedef fostlib::tagged_string< tag_type, fostlib::string > tstring;


FSL_TEST_FUNCTION( constructors ) {
    fostlib::test::default_copy_constructable< tstring >();

    const wchar_t *uenc = L"A string", *enc = L"A   s t r i n g ";

    tstring tt1a( enc ), tt1b = tstring( tstring::impl_type( enc ) );
    tstring tt2a( uenc, tstring::unencoded ), tt2b( tstring::impl_type( uenc ), tstring::unencoded );
    tstring tt3a( enc, tstring::encoded ), tt3b( tstring::impl_type( enc ), tstring::encoded );

    FSL_CHECK_EQ( tt1a.underlying(), enc );
    FSL_CHECK_EQ( tt1b.underlying(), enc );
    FSL_CHECK_EQ( tt2a.underlying(), enc );
    FSL_CHECK_EQ( tt2b.underlying(), enc );
    FSL_CHECK_EQ( tt3a.underlying(), enc );
    FSL_CHECK_EQ( tt3b.underlying(), enc );
}


FSL_TEST_FUNCTION( ascii ) {
    fostlib::test::default_copy_constructable< fostlib::ascii_string >();

    FSL_CHECK_NOTHROW( fostlib::ascii_string a( "abc", fostlib::ascii_string::encoded ) );
    FSL_CHECK_EXCEPTION( fostlib::ascii_string a( "x\xbbx" ), fostlib::exceptions::out_of_range< int > );
}

