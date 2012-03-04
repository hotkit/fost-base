/*
    Copyright 2009-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core-test.hpp"
#include <fost/detail/tagged-string.hpp>

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
    std::string str = "x\xbbx";
    FSL_CHECK_EXCEPTION( fostlib::ascii_string a( str ), fostlib::exceptions::out_of_range< int > );
}


FSL_TEST_FUNCTION(coerce) {
    fostlib::utf8_string source("source");
    std::vector< fostlib::utf8 > to(
        fostlib::coerce< std::vector< fostlib::utf8 > >(source));
    FSL_CHECK_EQ(to.size(), 6u);
    FSL_CHECK_EQ(to[0], 's');
    FSL_CHECK_EQ(to[1], 'o');
    FSL_CHECK_EQ(to[2], 'u');
    FSL_CHECK_EQ(to[3], 'r');
    FSL_CHECK_EQ(to[4], 'c');
    FSL_CHECK_EQ(to[5], 'e');
}

