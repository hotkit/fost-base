/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INSTANCE_HPP
#define FOST_INSTANCE_HPP
#pragma once


#include <fost/schema.hpp>


namespace fostlib {


    class instance_base : boost::noncopyable {
    public:
        instance_base( const meta_instance & );

        accessors< const meta_instance & > _meta;
    };


}


#endif // FOST_SCHEMA_HPP
