/**
    Copyright 2010-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_INSERT_HPP
#define FOST_INSERT_HPP
#pragma once


#include <fost/json.hpp>


namespace fostlib {


    /// Insert a value into a set or multiset
    template<typename S, typename V>
    inline auto insert(S &s, V &&v) {
        return s.insert(std::forward<V>(v));
    }

    /// Insert a value at the requested location wihin a JSON blob
    template<typename V>
    inline json &insert(json &js, jcursor const &p, V &&v) {
        return p.insert(js, coerce<json>(std::forward<V>(v)));
    }
    template<typename C, typename V>
    inline json &insert(json &js, C &&p, V &&v) {
        return jcursor{std::forward<C>(p)}.insert(
                js, coerce<json>(std::forward<V>(v)));
    }
    /// Allow up to any length
    template<typename C1, typename C2, typename... C>
    inline json &insert(json &js, jcursor jc, C1 &&p1, C2 &&p2, C &&... p) {
        return insert(
                js, jc /= std::forward<C1>(p1), std::forward<C2>(p2),
                std::forward<C>(p)...);
    }
    template<typename C0, typename C1, typename C2, typename... C>
    inline json &insert(json &js, C0 &&p0, C1 &&p1, C2 &&p2, C &&... p) {
        return insert(
                js, jcursor{std::forward<C0>(p0)}, std::forward<C1>(p1),
                std::forward<C2>(p2), std::forward<C>(p)...);
    }


}


#endif // FOST_INSERT_HPP
