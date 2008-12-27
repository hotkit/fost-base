/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet-test.hpp"
#include <fost/internet/smtp.hpp>


using namespace fostlib;


FSL_TEST_SUITE( smtp );


FSL_TEST_FUNCTION( basic ) {
    rfc822_address kirit( L"kirit_nospam@felspar.com" );
}
