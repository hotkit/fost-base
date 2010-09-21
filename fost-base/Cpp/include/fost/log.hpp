/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_LOG_HPP
#define FOST_LOG_HPP
#pragma once


#include <fost/yaml.hpp>


namespace fostlib {


    /// Send a YAML record to the log sinks
    FOST_CORE_DECLSPEC void log( const yaml::record & );


}


#endif // FOST_LOG_HPP
