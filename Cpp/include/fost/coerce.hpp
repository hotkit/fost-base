/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
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


    template< typename T, typename F, typename Enable = void >
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


}


#ifdef FOST_STRING_HPP
    #include <fost/detail/coerce.hpp>
#endif


#endif // FOST_COERCE_HPP
