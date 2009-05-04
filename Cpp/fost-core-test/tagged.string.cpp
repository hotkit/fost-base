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
    static void do_encode( fostlib::string from, fostlib::string &into ) {
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

    tstring tt1;
    tstring tt2u( uenc, tstring::unencoded ), tt3u( tstring::impl_type( uenc ), tstring::unencoded );
    tstring tt2e( enc, tstring::encoded ), tt3e( tstring::impl_type( enc ), tstring::encoded );

    FSL_CHECK_EQ( tt2u.underlying(), enc );
    FSL_CHECK_EQ( tt2e.underlying(), enc );
    FSL_CHECK_EQ( tt3u.underlying(), enc );
    FSL_CHECK_EQ( tt3e.underlying(), enc );
}

