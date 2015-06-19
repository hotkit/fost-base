/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_MODULE_HPP
#define FOST_MODULE_HPP
#pragma once


#include <fost/json.hpp>


namespace fostlib {


    /// Represents a code module
    class module {
        const module *parent;
        const string name;
        mutable jcursor jc;
        mutable string str;
    public:
        /// Create a new root module
        explicit module(const string &);
        /// Create a sub-module
        module(const module &, const string &);

        /// The path for the module as a jcursor
        operator const jcursor & () const;
        /// The path for the module as a string
        operator const string & () const;
    };


    /// The fost module
    extern const module c_fost;
    /// The fost-base module
    extern const module c_fost_base;
    /// The fost-base fost-core module
    extern const module c_fost_base_core;


}


#endif // FOST_MODULE_HPP
