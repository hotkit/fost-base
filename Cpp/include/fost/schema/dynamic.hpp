/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_DYNAMIC_HPP
#define FOST_SCHEMA_DYNAMIC_HPP
#pragma once


#include <fost/schema/base.hpp>


namespace fostlib {


    class meta_instance : boost::noncopyable {
    public:
        explicit meta_instance( const string &name );
        virtual ~meta_instance() {}

        accessors< const string > name;

        virtual string table( const instance_base & ) const;
    };


}


#endif // FOST_SCHEMA_DYNAMIC_HPP
