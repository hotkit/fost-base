/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_VARIANT_HPP
#define FOST_VARIANT_HPP
#pragma once


#include <fost/config.hpp>
#include <boost/variant.hpp>
#include <fost/string.hpp>
#include <fost/coerce/nullable.hpp>


namespace fostlib {


    class FOST_CORE_DECLSPEC variant {
        boost::variant< t_null, bool, int64_t, double, string > m_v;
    public:
        variant() : m_v( null ) {}
        explicit variant( bool b ) : m_v( b ) {}
        explicit variant( char c ) : m_v( int64_t( c ) ) {}
        explicit variant( int i ) : m_v( int64_t( i ) ) {}
        explicit variant( unsigned int i ) : m_v( int64_t( i ) ) {}
#ifdef FOST_USE_LONG
        explicit variant( long l ) : m_v( int64_t( l ) ) {}
        explicit variant( unsigned long l ) : m_v( int64_t( l ) ) {}
#endif
        explicit variant( int64_t i ) : m_v( i ) {}
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
        template<>
        nullable< long > get() const {
            return coerce< nullable< long > >( get< int64_t >() );
        }
#endif

        bool operator ==( const variant &v ) const;
        bool operator !=( const variant &v ) const { return !( *this == v ); }

        template< typename T > variant &operator =( T t ) { m_v = variant( t ); return *this; }
        variant &operator =( const string &s ) { m_v = s; return *this; }

        template< typename T >
        typename T::result_type apply_visitor( T &t ) const {
            return boost::apply_visitor( t, m_v );
        }
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer< bool, variant > {
        bool coerce( const variant &f );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< int64_t, variant > {
        int64_t coerce( const variant &f );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< double, variant > {
        double coerce( const variant &f );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, variant > {
        string coerce( const variant &f );
    };


}


namespace std {


    inline fostlib::ostream &operator <<( fostlib::ostream &o, const fostlib::variant &v ) {
        return o << fostlib::coerce< fostlib::string >( v );
    }


}


#endif // FOST_VARIANT_HPP
