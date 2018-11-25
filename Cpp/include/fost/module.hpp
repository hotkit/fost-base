/**
    Copyright 2015-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
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
        : m_parent(nullptr), m_name(n) {}
        /// Create a sub-module
        constexpr module(const module &p, const nliteral &n)
        : m_parent(&p), m_name(n) {}
        /// Copy needs to deal properly with the name_str member
        module(const module &m)
        : m_parent(m.m_parent),
          m_name_str(m.m_name_str),
          m_name(m_name_str ? m_name_str->c_str() : m.m_name) {}
        /// Make movable
        module(module &&m)
        : m_parent(m.m_parent),
          m_name_str(std::move(m.m_name_str)),
          m_name(m_name_str ? m_name_str->c_str() : m.m_name) {}
        /// Create dynamic modules
        module(const module &p, std::string n)
        : m_parent(&p), m_name_str(std::move(n)), m_name(m_name_str->c_str()) {}

        /// Make the name accessible
        nliteral name() const { return m_name; }
        /// Make the parent accessible
        const module *parent() const { return m_parent; }

        /// The path for the module as a json
        json as_json() const;
        operator json() const { return as_json(); }
        /// The path for the module as a jcursor
        jcursor as_jcursor() const;
        operator jcursor() const { return as_jcursor(); }
        /// The path for the module as a string
        string as_string() const;
        operator string() const { return as_string(); }

        /// Allow modules to be compared for equality
        bool operator==(const module &m) const { return this == &m; }

      private:
        const module *const m_parent;
        const fostlib::nullable<std::string> m_name_str;
        const nliteral m_name;
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
        json coerce(const module &m) const { return m.as_json(); }
    };


}


namespace std {
    inline std::ostream &operator<<(std::ostream &o, const fostlib::module &m) {
        return o << m.as_string();
    }
}


#endif // FOST_MODULE_HPP
