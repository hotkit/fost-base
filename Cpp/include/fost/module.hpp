/*
    Copyright 2015-2017, Felspar Co Ltd. http://support.felspar.com/
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
    class module final {
    public:
        /// Create a new root module
        constexpr explicit module(const nliteral &n)
        : parent(nullptr), name(n) {
        }
        /// Create a sub-module
        constexpr module(const module &p, const nliteral &n)
        : parent(&p), name(n) {
        }
        /// Copy needs to deal properly with the name_str member
        module(const module &m)
        : parent(m.parent),
            name_str(m.name_str),
            name(name_str ? name_str.value().c_str() : m.name)
        {
        }
        /// Make movable
        module(module &&m)
        : parent(m.parent),
            name_str(std::move(m.name_str)),
            name(name_str ? name_str.value().c_str() : m.name)
        {
        }
        /// Create dynamic modules
        module(const module &p, std::string n)
        : parent(&p), name_str(std::move(n)), name(name_str.value().c_str()) {
        }

        /// The path for the module as a json
        json as_json() const;
        operator json () const {
            return as_json();
        }
        /// The path for the module as a jcursor
        jcursor as_jcursor() const;
        operator jcursor () const {
            return as_jcursor();
        }
        /// The path for the module as a string
        string as_string() const;
        operator string () const {
            return as_string();
        }

        /// Allow modules to be compared for equality
        bool operator == (const module &m) const {
            return this == &m;
        }

    private:
        const module * const parent;
        const fostlib::nullable<std::string> name_str;
        const nliteral name;
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
            return static_cast<json>(m);
        }
    };


}


namespace std {
    inline
    std::ostream &operator << (std::ostream &o, const fostlib::module &m) {
        return o << static_cast<fostlib::string>(m);
    }
}


#endif // FOST_MODULE_HPP
