/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_FIELDS_HPP
#define FOST_SCHEMA_FIELDS_HPP
#pragma once


#include <fost/schema/base.hpp>


namespace fostlib {


    template< typename value_type >
    class field_wrapper : public field_base {
    public:
        field_wrapper( const string &type_name )
        : field_base( type_name ) {
        }
    };


    extern const field_wrapper< int64_t > integer;


}


#endif // FOST_SCHEMA_FIELDS_HPP
