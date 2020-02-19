/**
    Copyright 2010-2020 Red Anchor Trading Co. Ltd.

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
    inline json &push_back(json &j, V &&v) {
        jcursor().push_back(j, coerce<json>(std::forward<V>(v)));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location
    /// with a blob
    template<typename V>
    inline json &push_back(json &j, jcursor const &jc, V &&v) {
        jc.push_back(j, coerce<json>(std::forward<V>(v)));
        return j;
    }
    template<typename C, typename V>
    inline json &push_back(json &j, C &&p, V &&v) {
        jcursor{std::forward<C>(p)}.push_back(
                j, coerce<json>(std::forward<V>(v)));
        return j;
    }
    /// Allow for any length
    template<typename C1, typename C2, typename... C>
    inline json &push_back(json &j, jcursor jc, C1 &&p1, C2 &&p2, C &&... p) {
        return push_back(
                j, jc /= std::forward<C1>(p1), std::forward<C2>(p2),
                std::forward<C>(p)...);
    }
    template<typename C0, typename C1, typename C2, typename... C>
    inline json &push_back(json &j, C0 &&jc, C1 &&p1, C2 &&p2, C &&... p) {
        return push_back(
                j, jcursor{std::forward<C0>(jc)}, std::forward<C1>(p1),
                std::forward<C2>(p2), std::forward<C>(p)...);
    }

    /// Allow us to push a value to the back of a container
    template<typename C, typename V>
    C &push_back(C &c, V &&v) {
        c.push_back(std::forward<V>(v));
        return c;
    }

    /// Allow us to push a value to the back of a JSON array
    template<typename V>
    json::array_t &push_back(json::array_t &a, V &&v) {
        a.push_back(std::forward<V>(v));
        return a;
    }


}


#endif // FOST_PUSH_BACK_HPP
