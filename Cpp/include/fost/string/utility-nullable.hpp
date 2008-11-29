/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_UTILITY_NULLABLE_HPP
#define FOST_STRING_UTILITY_NULLABLE_HPP
#pragma once


#include <fost/string.hpp>
#include <fost/nullable.hpp>


namespace fostlib {


    FOST_CORE_DECLSPEC nullable< string > concat( const nullable< string > &left, const string &mid, const nullable< string > &right );
    FOST_CORE_DECLSPEC nullable< string > concat( const nullable< string > &left, const nullable< string > &right );

    FOST_CORE_DECLSPEC nullable< utf8string > trim( const nullable< utf8string > &text );

    FOST_CORE_DECLSPEC nullable< string > trim( const nullable< string > &text );


    FOST_CORE_DECLSPEC utf8string replaceAll( const utf8string &text, const utf8string &findThis, const nullable< utf8string > &replaceWith );

    FOST_CORE_DECLSPEC string replaceAll( const string &text, const string &findThis, const nullable< string > &replaceWith );
    FOST_CORE_DECLSPEC nullable< string > replaceAll( const nullable< string > &text, const string &findThis, const nullable< string > &replaceWith );


    FOST_CORE_DECLSPEC std::pair< utf8string, nullable< utf8string > > partition( const utf8string &text, const utf8string &bound );
    FOST_CORE_DECLSPEC std::pair< utf8string, nullable< utf8string > > partition( const nullable< utf8string > &text, const utf8string &bound );
    FOST_CORE_DECLSPEC std::pair< utf8string, nullable< utf8string > > partition( const utf8string &text );
    FOST_CORE_DECLSPEC std::pair< utf8string, nullable< utf8string > > partition( const nullable< utf8string > &text );

    FOST_CORE_DECLSPEC std::pair< string, nullable< string > > partition( const string &text, const string &bound );
    FOST_CORE_DECLSPEC std::pair< string, nullable< string > > partition( const nullable< string > &text, const string &bound );
    inline std::pair< string, nullable< string > > partition( wliteral text, const string &str ) {
        return partition( string( text ), str );
    }
    FOST_CORE_DECLSPEC std::pair< string, nullable< string > > partition( const string &text );
    FOST_CORE_DECLSPEC std::pair< string, nullable< string > > partition( const nullable< string > &text );

    FOST_CORE_DECLSPEC std::pair< string, nullable< string > > crack( const string &text, const string &open, const string &close );


}


#endif // FOST_STRING_UTILITY_NULLABLE_HPP

