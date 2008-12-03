/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_BASE_HPP
#define FOST_SCHEMA_BASE_HPP
#pragma once


#include <fost/core>
#include <fost/thread.hpp>


namespace fostlib {


    class FOST_SCHEMA_DECLSPEC field_base : boost::noncopyable {
    public:
        field_base( const string &type_name );
        virtual ~field_base();

        accessors< const string > type_name;

        static const field_base &fetch( const string &type_name );
    };


    class FOST_SCHEMA_DECLSPEC attribute_base : boost::noncopyable {
    protected:
        virtual ~attribute_base() {}
    };


}


#endif // FOST_SCHEMA_BASE_HPP
