/*
    Copyright 2010-2011, Felspar Co Ltd. http://support.felspar.com/
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

    /// Insert a JSON value at the requested location within another JSON blob
    inline fostlib::json &insert(
        fostlib::json &json, const fostlib::jcursor &p, const fostlib::json &value
    ) {
        return p.insert(json, value);
    }

    /// Insert a value at the requested location wihin a JSON blob
    template<typename JC, typename V>
    inline fostlib::json &insert(
        fostlib::json &json, const JC &p, const V &v
    ) {
        return insert(json, fostlib::jcursor(p), fostlib::coerce<fostlib::json>(v));
    }
    /// Insert a value at the requested location wihin a JSON blob
    template<typename JC, typename P1, typename V>
    inline fostlib::json &insert(
        fostlib::json &json, const JC &r, const P1 &p1, const V &v
    ) {
        return insert(json, fostlib::jcursor(r) / p1, fostlib::coerce<fostlib::json>(v));
    }
    /// Insert a value at the requested location wihin a JSON blob
    template<typename JC, typename P1, typename P2, typename V>
    inline fostlib::json &insert(
        fostlib::json &json, const JC &r, const P1 &p1, const P2 &p2, const V &v
    ) {
        return insert(json, fostlib::jcursor(r) / p1 / p2, fostlib::coerce<fostlib::json>(v));
    }


}


#endif // FOST_INSERT_HPP