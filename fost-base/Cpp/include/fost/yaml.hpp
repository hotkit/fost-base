/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_YAML_HPP
#define FOST_YAML_HPP
#pragma once


#include <fost/config.hpp>


namespace fostlib {


    namespace yaml {


        /// A simple YAML record for output only
        class FOST_CORE_DECLSPEC record {
            std::list< std::pair< std::size_t, string > > content;
            public:
                /// Add a field without a value
                record &add( const string &key, t_null = null );
        };


    }


}


#endif // FOST_YAML_HPP
