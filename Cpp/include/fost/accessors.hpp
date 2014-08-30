/*
    Copyright 2001-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_ACCESSORS_HPP
#define FOST_ACCESSORS_HPP
#pragma once


#include "config.hpp"


namespace fostlib {


    struct lvalue {};
    struct rvalue {};

    template< typename V, typename accessor_impl = rvalue >
    class accessors;

    template< typename V >
    class accessors< V, rvalue > {
        V m_v;
    public:
#ifdef FOST_HAS_MOVE
        template<typename... A>
        explicit accessors(A&&... args)
        : m_v(std::forward<A>(args)...) {
        }
#else
        accessors()
        : m_v() {
        }
        explicit accessors(const V &v)
        : m_v(v) {
        }
#endif

        const V &operator() () const { return m_v; }
        void operator() ( const V &v ) { m_v = v; }
    };

    template< typename V >
    class accessors< V, lvalue > {
        V m_v;
    public:
#ifdef FOST_HAS_MOVE
        template<typename... A>
        explicit accessors(A&&... args)
        : m_v(std::forward<A>(args)...) {
        }
#else
        accessors()
        : m_v() {
        }
        explicit accessors(const V &v)
        : m_v(v) {
        }
#endif

        const V &operator() () const { return m_v; }
        V &operator() () { return m_v; }
        void operator() ( const V &v ) { m_v = v; }
    };

    template< typename V >
    class accessors< boost::scoped_ptr< V >, rvalue > {
        boost::scoped_ptr< V > m_v;
    public:
        explicit accessors() : m_v() {}
        explicit accessors( V *v ) : m_v( v ) {}

        const boost::scoped_ptr< V > &operator() () const { return m_v; }
        void operator() ( const boost::scoped_ptr< V > &v ) { m_v = v; }
    };
    template< typename V >
    class accessors< const boost::scoped_ptr< V >, rvalue > {
        const boost::scoped_ptr< V > m_v;
    public:
        explicit accessors( V *v ) : m_v( v ) {}

        const boost::scoped_ptr< V > &operator() () const { return m_v; }
    };


}


#ifdef FOST_STRING_HPP
    #include <fost/detail/accessors.hpp>
#endif


#endif // FOST_ACCESSORS_HPP

