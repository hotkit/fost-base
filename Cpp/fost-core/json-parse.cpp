/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/json/parse.hpp>


const fostlib::json_string_parser fostlib::json_string_p;
const fostlib::json_parser fostlib::json_p;


#include <fost/exception/parse_error.hpp>


fostlib::json fostlib::json::parse( const string &toparse ) {
    fostlib::json ret;
    if ( !boost::spirit::parse( coerce< std::wstring >( toparse ).c_str(), fostlib::json_p[ phoenix::var( ret ) = phoenix::arg1 ] ).full )
        throw fostlib::exceptions::parse_error( L"Whilst parsing JSON string", toparse );
    return ret;
}

fostlib::json fostlib::json::parse( const string &toparse, const json &def ) {
    fostlib::json ret;
    if ( !boost::spirit::parse( coerce< std::wstring >( toparse ).c_str(), fostlib::json_p[ phoenix::var( ret ) = phoenix::arg1 ] ).full )
        return def;
    else
        return ret;
}
