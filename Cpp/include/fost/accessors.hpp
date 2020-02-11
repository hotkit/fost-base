/**
    Copyright 2001-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_ACCESSORS_HPP
#define FOST_ACCESSORS_HPP
#pragma once


namespace fostlib {


    struct lvalue {};
    struct rvalue {};

    template<typename V, typename accessor_impl = rvalue>
    class accessors;

    template<typename V>
    class accessors<V, rvalue> {
        V m_v;

      public:
        template<typename... A>
        explicit accessors(A &&... args) : m_v(std::forward<A>(args)...) {}

        const V &operator()() const { return m_v; }
        void operator()(const V &v) { m_v = v; }
    };

    template<typename V>
    class accessors<V, lvalue> {
        V m_v;

      public:
        template<typename... A>
        explicit accessors(A &&... args) : m_v(std::forward<A>(args)...) {}

        const V &operator()() const { return m_v; }
        V &operator()() { return m_v; }
        void operator()(const V &v) { m_v = v; }
    };


}


#endif // FOST_ACCESSORS_HPP
