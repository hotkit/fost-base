/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_VARIANT_CORE_HPP
#define FOST_VARIANT_CORE_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/string.hpp>
#include <fost/nullable-core.hpp>

#include <boost/variant.hpp>


namespace fostlib {


    class FOST_CORE_DECLSPEC variant {
        boost::variant< t_null, bool, int64_t, double, string > m_v;
    public:
        variant() : m_v( null ) {}
        explicit variant( bool b ) : m_v( b ) {}
        template<typename I>
        explicit variant(I i, std::enable_if_t<std::is_integral<I>::value, void*> = nullptr)
        : m_v(int64_t(i)) {
        }
        explicit variant( float f ) : m_v( double( f ) ) {}
        explicit variant( double d ) : m_v( d ) {}
        explicit variant( nliteral s ) : m_v( string( s ) ) {}
        explicit variant( wliteral s ) : m_v( string( s ) ) {}
        explicit variant( const string &s ) : m_v( s ) {}

        bool isnull() const;

        template< typename T >
        nullable< T > get() const {
            const T *p = boost::get< T >( &m_v );
            if ( p )
                return *p;
            else
                return null;
        }
#ifdef FOST_USE_LONG
        template<> nullable< long > get() const;
#endif

        /// Allow variants to be compared
        bool operator ==( const variant &v ) const;
        /// Allow variants to be compared
        bool operator !=( const variant &v ) const {
            return !( *this == v );
        }

        /// Allow assignment
        template< typename T > variant &operator =( T t ) {
            m_v = variant(t).m_v; return *this;
        }
        /// Allow assignment
        variant &operator =( const string &s ) {
            m_v = s; return *this;
        }

        template< typename T >
        typename T::result_type apply_visitor( T &t ) const {
            return boost::apply_visitor( t, m_v );
        }
    };


}


#endif // FOST_VARIANT_CORE_HPP
