/*
    Copyright  2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_ACCESSORS_HPP
#define FOST_STRING_ACCESSORS_HPP
#pragma once


#include <fost/accessors.hpp>
#include <fost/string.hpp>


namespace fostlib {


    template<>
    class accessors<string, rvalue> {
        fostlib::string m_v;

      public:
        accessors() : m_v() {}
        accessors(wliteral s) : m_v(s) {}
        accessors(const string &v) : m_v(v) {}

        const string &operator()() const { return m_v; }
        void operator()(const string &v) { m_v = v; }
    };
    template<>
    class accessors<fostlib::string, lvalue> {
        fostlib::string m_v;

      public:
        accessors() : m_v() {}
        accessors(wliteral s) : m_v(s) {}
        accessors(const string &v) : m_v(v) {}

        const string &operator()() const { return m_v; }
        string &operator()() { return m_v; }
        void operator()(const string &v) { m_v = v; }
    };


}


#endif // FOST_STRING_ACCESSORS_HPP
