/*
    Copyright 2001-2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_NULLABLE_CORE_HPP
#define FOST_NULLABLE_CORE_HPP
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


#endif // FOST_NULLABLE_HPP

