/*
    Copyright  2001-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_UTILITY_HPP
#define FOST_STRING_UTILITY_HPP
#pragma once


#include <fost/string.hpp>


namespace fostlib {


    typedef std::vector< string > split_type;
    FOST_CORE_DECLSPEC split_type split( const string &text, const string &on );


    FOST_CORE_DECLSPEC string guid();

#ifdef WIN32
    FOST_CORE_DECLSPEC string format_last_error();
#endif


}


#endif // FOST_STRING_UTILITY_HPP

