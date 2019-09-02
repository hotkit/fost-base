/**
    Copyright 2010-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_PUSH_BACK_HPP
#define FOST_PUSH_BACK_HPP
#pragma once


namespace fostlib {


    /// Allow us to push any JSON constructable object to the end of the root of
    /// the JSON blob
    template<typename V>
    inline fostlib::json &push_back(fostlib::json &j, const V &v) {
        fostlib::jcursor().push_back(j, fostlib::json(v));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location
    /// with a blob
    template<typename C, typename V>
    inline fostlib::json &push_back(fostlib::json &j, const C &p, const V &v) {
        fostlib::jcursor(p).push_back(j, fostlib::json(v));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location
    /// with a blob
    template<typename C1, typename C2, typename V>
    inline fostlib::json &
            push_back(fostlib::json &j, const C1 &p1, const C2 &p2, const V &v) {
        (fostlib::jcursor(p1) / p2).push_back(j, fostlib::json(v));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location
    /// with a blob
    template<typename C1, typename C2, typename C3, typename V>
    inline fostlib::json &push_back(
            fostlib::json &j,
            const C1 &p1,
            const C2 &p2,
            const C3 &p3,
            const V &v) {
        (fostlib::jcursor(p1) / p2 / p3).push_back(j, fostlib::json(v));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location
    /// with a blob
    template<typename C1, typename C2, typename C3, typename C4, typename V>
    inline fostlib::json &push_back(
            fostlib::json &j,
            const C1 &p1,
            const C2 &p2,
            const C3 &p3,
            const C4 &p4,
            const V &v) {
        (fostlib::jcursor(p1) / p2 / p3 / p4).push_back(j, fostlib::json(v));
        return j;
    }

    /// Allow for any length
    template<typename C1, typename C2, typename... C>
    inline fostlib::json &push_back(
            fostlib::json &j,
            fostlib::jcursor jc,
            const C1 &p1,
            const C2 &p2,
            C &&... p) {
        return push_back(j, jc / p1, p2, std::forward<C>(p)...);
    }

    /// Allow us to push a value to the back of a container
    template<typename C, typename V>
    C &push_back(C &c, const V &v) {
        c.push_back(v);
        return c;
    }

    /// Allow us to push a value to the back of a JSON array
    template<typename V>
    json::array_t &push_back(json::array_t &a, const V &v) {
        a.push_back(v);
        return a;
    }


}


#endif // FOST_PUSH_BACK_HPP
