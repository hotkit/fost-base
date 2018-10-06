/**
    Copyright 2008-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_COERCE_HPP
#define FOST_COERCE_HPP
#pragma once


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


#endif // FOST_COERCE_HPP
