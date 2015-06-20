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

        /// Allow modules to be compared for equality
        bool operator == (const module &m) const {
            return this == &m;
        }
    };


    /// The fost module
    extern const module c_fost;
    /// The fost-base module
    extern const module c_fost_base;
    /// The fost-base fost-core module
    extern const module c_fost_base_core;


    /// Allow coercion to JSON
    template<>
    struct coercer<json, module> {
        json coerce(const module &m) const {
            return fostlib::coerce<json>(static_cast<const jcursor &>(m));
        }
    };


}


namespace std {
    inline
    std::ostream &operator << (std::ostream &o, const fostlib::module &m) {
        return o << static_cast<const fostlib::string &>(m);
    }
}


#endif // FOST_MODULE_HPP
