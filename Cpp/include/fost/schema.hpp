/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_HPP
#define FOST_SCHEMA_HPP
#pragma once


#include <fost/core>


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


    class field_base : boost::noncopyable {
    protected:
        explicit field_base
        virtual ~field_base() throw () {}

    public:
        virtual string name() const = 0;
    };


    class FOST_SCHEMA_DECLSPEC model_base : boost::noncopyable {
    public:
        explicit model_base( const string &name );
        model_base( const model &super_class, const string &name );
        virtual ~model_base() {}

        accessors< const string > name;

        virtual ~model_base() throw () {}
    };


    class meta_instance : boost::noncopyable {
    public:
        virtual ~meta_instance() {}

        virtual string table() const = 0;
    };


}


#endif // FOST_SCHEMA_HPP
