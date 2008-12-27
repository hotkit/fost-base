/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/smtp.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;


/*
    fostlib::rfc822_address
*/


fostlib::rfc822_address::rfc822_address() {
}


fostlib::rfc822_address::rfc822_address( const string &e, const nullable<string> &n )
: email( e ), name( n ) {
    if ( email().empty() )
        throw exceptions::null( L"Email address is empty" );
    if ( email().find( '@' ) == string::npos )
        throw exceptions::parse_error( L"Email address doesn't contain @ symbol", email() );
}
