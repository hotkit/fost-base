/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/string/tagged-string.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>


void fostlib::ascii_string_tag::do_encode( fostlib::nliteral from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_string_tag::do_encode( fostlib::nliteral from, std::string &into )" );
}
void fostlib::ascii_string_tag::do_encode( const std::string &from, std::string &into ) {
    throw fostlib::exceptions::not_implemented( L"fostlib::ascii_string_tag::do_encode( const std::string &from, std::string &into )" );
}
void fostlib::ascii_string_tag::check_encoded( const std::string &s ) {
    for ( std::string::const_iterator c( s.begin() ); c != s.end(); ++c )
        if ( *c < 1 || *c > 127 )
            throw fostlib::exceptions::out_of_range< int >( L"ASCII characters outside valid range", 1, 127, *c );
}
