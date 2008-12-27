/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet-test.hpp"


using namespace fostlib;


FSL_TEST_SUITE( internet_host );


FSL_TEST_FUNCTION( constructors ) {
    FSL_CHECK_EQ( host().name(), L"" );
    FSL_CHECK_EQ( host().service(), L"0" );
    FSL_CHECK_EQ( host( L"localhost" ).name(), L"localhost" );
    FSL_CHECK_EQ( host( 127, 0, 0, 1 ).name(), L"127.0.0.1" );
    FSL_CHECK_EQ( host( 127 << 24 ).name(), L"127.0.0.0" );
}
