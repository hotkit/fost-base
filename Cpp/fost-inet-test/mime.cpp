/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet-test.hpp"
#include <fost/internet/mime.hpp>


using namespace fostlib;


FSL_TEST_SUITE( mime );


FSL_TEST_FUNCTION( headers ) {
    mime::mime_headers headers;
    headers.parse( L"X-First: value\r\nX-Second: value" );
    FSL_CHECK( headers.exists( L"X-First" ) );
    FSL_CHECK( headers.exists( L"X-Second" ) );
    FSL_CHECK( !headers.exists( L"X-Third" ) );

    FSL_CHECK_EQ( headers[ L"X-First" ].value(), L"value" );
    FSL_CHECK_EQ( headers[ L"X-Second" ].value(), L"value" );
}
