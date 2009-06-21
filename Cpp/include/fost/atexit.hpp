/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_ATEXIT_HPP
#define FOST_ATEXIT_HPP
#pragma once


#include <fost/config.hpp>
#include <boost/function.hpp>


namespace fostlib {


    FOST_CORE_DECLSPEC void atexit( boost::function< void( void ) > function );


}


#endif // FOST_ATEXIT_HPP
