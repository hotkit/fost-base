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


    class instance_base;

    struct persistent : boost::noncopyable {
        persistent() throw () {}
        virtual ~persistent() throw() {}

        virtual void kill() = 0;
        virtual void commit() = 0;

        class iterator {
        };
        iterator begin() const;
        iterator end() const;
    };


    class attribute_base : private persistent {
    public:
    };


    class instance_base : private persistent {
    public:
    };


}


#endif // FOST_SCHEMA_HPP
