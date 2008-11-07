/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_NULLABLE_HPP
#define FOST_NULLABLE_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/null.hpp>


namespace fostlib {


    template < typename T >
    class nullable : private std::pair< bool, T > {
        typedef std::pair< bool, T > superclass_t;
    public:
        typedef T t_value;

        inline nullable();
        inline nullable( const nullable & );
        inline nullable( const T & );
        inline nullable( t_null );

        template< typename Y >
        nullable( const nullable< Y > &y )
        : std::pair< bool, T >( y.isnull(), y.isnull() ? T() : T( y.value() ) ) {
        }

        inline ~nullable();

        inline bool isnull() const;

        inline nullable &operator =( const T & );
        inline nullable &operator =( t_null );
        inline bool operator ==( const nullable< T > &rhs ) const;
        inline bool operator ==( const T &rhs ) const;
        inline bool operator !=( const nullable< T > &rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const T &rhs ) const { return !( *this == rhs ); }

        inline void set_null();

        inline const T &value() const;
        inline T &value();
        inline const T &value( const T &value ) const;
    };


}


namespace std {
    // Specialisation to allow fostlib::nullables to be used as keys in sets and maps.
    template< typename Y >
    struct less< fostlib::nullable< Y > > {
        bool operator ()( const fostlib::nullable< Y > &l, const fostlib::nullable< Y > &r ) const {
            if ( l.isnull() )
                return !r.isnull();
            else if ( r.isnull() )
                return false;
            else
                return std::less< Y >()( l.value(), r.value() );
        }
    };
}


#ifdef FOST_STRING_HPP
    #include <fost/string/nullable.hpp>
#endif


#include <fost/exception/null.hpp>


template< typename T > inline
fostlib::nullable< T >::nullable()
: superclass_t( true, T() ) {
}

template< typename T > inline
fostlib::nullable< T >::nullable( const nullable &t )
: superclass_t( t.first, t.second ) {
}

template< typename T > inline
fostlib::nullable< T >::nullable( const T &t )
: superclass_t( false, t ) {
}

template< typename T > inline
fostlib::nullable< T >::nullable( t_null )
: superclass_t( true, T() ) {
}

template< typename T > inline
fostlib::nullable< T >::~nullable() {
}


template< typename T > inline
bool fostlib::nullable< T >::isnull() const {
    return superclass_t::first;
}
template< typename T > inline
void fostlib::nullable< T >::set_null() {
    superclass_t::first = true;
    superclass_t::second = T();
}


template< typename T > inline
fostlib::nullable< T > &fostlib::nullable< T >::operator =( const T &t ) {
    superclass_t::first = false;
    superclass_t::second = t;
    return *this;
}
template< typename T > inline
fostlib::nullable< T > &fostlib::nullable< T >::operator =( t_null ) {
    set_null();
    return *this;
}


template< typename T > inline
bool fostlib::nullable< T >::operator ==( const T &rhs ) const {
    if ( isnull() )
        return false;
    else
        return value() == rhs;
}
template< typename T > inline
bool fostlib::nullable< T >::operator ==( const fostlib::nullable< T > &rhs ) const {
    if ( isnull() && rhs.isnull() )
        return true;
    else if ( !isnull() && !rhs.isnull() && value() == rhs.value() )
        return true;
    return false;
}


template< typename T > inline
const T &fostlib::nullable< T >::value() const {
    if ( isnull() )
        throw fostlib::exceptions::null();
    return superclass_t::second;
}
template< typename T > inline
T &fostlib::nullable< T >::value() {
    if ( isnull() )
        throw fostlib::exceptions::null();
    return superclass_t::second;
}
template< typename T > inline
const T &fostlib::nullable< T >::value( const T &v ) const {
    if ( isnull() )
        return v;
    else
        return superclass_t::second;
}


/*
	Comparison operators
*/


template< typename T > inline
bool operator ==( const T &lhs, const fostlib::nullable< T > &rhs ) {
	if ( rhs.isnull() )
		return false;
	else
		return lhs == rhs.value();
}


template< typename T > inline
bool operator ==( fostlib::t_null lhs, const fostlib::nullable< T > &rhs ) {
	return rhs.isnull();
}


template< typename T > inline
bool operator ==( const fostlib::nullable< T > &lhs, fostlib::t_null ) {
	return lhs.isnull();
}


template< typename L, typename R > inline
bool operator != ( const fostlib::nullable< L > &l, const fostlib::nullable< R > &r ) {
	if ( l.isnull() && r.isnull() )
        return false;
	else if ( l.isnull() || r.isnull() )
        return true;
	else
        return l.value() != r.value();
}


template< typename T > inline
bool operator !=( const T &lhs, const fostlib::nullable< T > &rhs ) {
	return !( lhs == rhs );
}


template< typename T > inline
bool operator !=( fostlib::t_null lhs, const fostlib::nullable< T > &rhs ) {
	return !( lhs == rhs );
}


template< typename T > inline
bool operator !=( const fostlib::nullable< T > &lhs, fostlib::t_null rhs ) {
	return !( lhs == rhs );
}


#endif // FOST_NULLABLE_HPP

