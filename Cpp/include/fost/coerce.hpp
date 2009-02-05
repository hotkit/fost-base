/*
	Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COERCE_HPP
#define FOST_COERCE_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/null.hpp>


namespace fostlib {


    template< typename T, typename F >
    struct coercer {
        // Not implemented -- must always be specialised
    };
    template< typename T >
    struct coercer< T, t_null > {
        // Cannot convert from null to any type
    };


    template< typename T, typename F >
    T coerce( const F &f ) {
        return coercer< T, F >().coerce( f );
    }


    template<>
    struct FOST_CORE_DECLSPEC coercer< int, int64_t > {
        int coerce( int64_t i );
    };

#ifdef FOST_USE_LONG
    template<>
    struct FOST_CORE_DECLSPEC coercer< long, int64_t > {
        long coerce( int64_t i );
    };
#endif


}


#ifdef FOST_STRING_HPP
    #include <fost/string/coerce.hpp>
#endif


#endif // FOST_COERCE_HPP
