/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-datetime.hpp"
#include <fost/detail/timestamp.hpp>


using namespace fostlib;


/*
    fostlib::rfc1123_timestamp
*/


void fostlib::rfc1123_timestamp_tag::do_encode( fostlib::nliteral from, ascii_string &into ) {
    throw exceptions::not_implemented("fostlib::rfc1123_timestamp_tag::do_encode( fostlib::nliteral from, ascii_string &into )");
}
void fostlib::rfc1123_timestamp_tag::do_encode( const ascii_string &from, ascii_string &into ) {
    throw exceptions::not_implemented("fostlib::rfc1123_timestamp_tag::do_encode( const ascii_string &from, ascii_string &into )");
}
void fostlib::rfc1123_timestamp_tag::check_encoded( const ascii_string &s ) {
    try {
        if ( s.underlying().length() != 31 )
            throw exceptions::out_of_range< int >("RFC 1123 date format must be 31 characters", 31, 31, s.underlying().length());
    } catch ( exceptions::exception &e ) {
        e.info() << "String is " << s << std::endl;
        throw;
    }
}
