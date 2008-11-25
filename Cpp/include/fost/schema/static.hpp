/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_STATIC_HPP
#define FOST_SCHEMA_STATIC_HPP
#pragma once


#include <fost/schema/dynamic.hpp>


namespace fostlib {


    template< typename model_type, typename superclass_type = t_null >
    class model : private model_base {
    public:
    };

    template< typename model, typename field_type >
    class field : private attribute_base {
    public:
    };


}


#endif // FOST_SCHEMA_STATIC_HPP
