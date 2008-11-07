/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_UTILITY_HPP
#define FOST_STRING_UTILITY_HPP
#pragma once


#include <fost/string.hpp>


namespace fostlib {


    // Removes whitespace from the start and end of a string
    FOST_CORE_DECLSPEC utf8string trim( const utf8string &text );
    FOST_CORE_DECLSPEC string trim( const string &text );
    inline string trim( wliteral text ) {
        return trim( string( text ) );
    }
    FOST_CORE_DECLSPEC string trim( const string &text, const string &chars );


    typedef std::vector< string > split_type;
    FOST_CORE_DECLSPEC split_type split( const string &text, const string &on );


    FOST_CORE_DECLSPEC string guid();


}


#endif // FOST_STRING_UTILITY_HPP

