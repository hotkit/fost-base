/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COERCE_NULLABLE_HPP
#define FOST_COERCE_NULLABLE_HPP
#pragma once


#include <fost/nullable.hpp>
#include <fost/coerce.hpp>
#include <fost/exception/null.hpp>


namespace fostlib {


    template< typename T, typename F >
    struct coercer< T, fostlib::nullable< F > > {
        T coerce( const nullable< F > &f ) {
            if ( f.isnull()  )
                throw exceptions::null();
            else
                return coercer< T, F >().coerce( f.value() );
        }
    };
    template< typename T, typename F >
    struct coercer< nullable< T >, F > {
        nullable< T > coerce( const F &f ) {
            return coercer< T, F >().coerce( f );
        }
    };
    template< typename T, typename F >
    struct coercer< nullable< T >, nullable< F > > {
        T coerce( const nullable< F > &f ) {
            if ( f.isnull() )
                return null;
            else
                return coercer< T, F >().coerce( f.value() );
        }
    };


}


#endif // FOST_COERCE_NULLABLE_HPP

