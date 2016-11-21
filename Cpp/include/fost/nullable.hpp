/*
    Copyright 2001-2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_NULLABLE_HPP
#define FOST_NULLABLE_HPP
#pragma once


#include <fost/nullable-core.hpp>


namespace std {
    /// Specialisation to allow fostlib::nullables to be used as keys in sets and maps.
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
    #include <fost/detail/nullable.hpp>
#endif


#include <fost/exception/null.hpp>


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
    if ( l.isnull() && r.isnull() ) return false;
    else if ( l.isnull() || r.isnull() ) return true;
    else return l.value() != r.value();
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

