/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INSERT_HPP
#define FOST_INSERT_HPP
#pragma once


#include <fost/json.hpp>


namespace fostlib {


    /// Insert a value into a set or multiset
    template<typename S>
    inline std::pair<typename S::const_iterator, bool> insert(
        S &s, const typename S::value_type &v
    ) {
        return s.insert(v);
    }

    /// Insert a value at the requested location wihin a JSON blob
    template<typename JC, typename V>
    inline fostlib::json &insert(
        fostlib::json &json, const JC &p, const V &v
    ) {
        return fostlib::jcursor(p).insert(json, fostlib::coerce<fostlib::json>(v));
    }
    /// Insert a value at the requested location wihin a JSON blob
    template<typename JC, typename P1, typename V>
    inline fostlib::json &insert(
        fostlib::json &json, const JC &r, const P1 &p1, const V &v
    ) {
        return (fostlib::jcursor(r) / p1).insert(json, fostlib::coerce<fostlib::json>(v));
    }
    /// Allow up to any length
    template<typename JC, typename C1, typename C2, typename... C>
    inline fostlib::json &insert(fostlib::json &j, const JC &jc,
            const C1 &p1, const C2 &p2, C &&... p) {
        return insert(j, fostlib::jcursor(jc) / p1, p2, std::forward<C>(p)...);
    }


}


#endif // FOST_INSERT_HPP
