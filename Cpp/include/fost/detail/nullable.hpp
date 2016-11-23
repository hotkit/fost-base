/*
    Copyright 2008-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_NULLABLE_HPP
#define FOST_STRING_NULLABLE_HPP
#pragma once


#include <fost/nullable.hpp>
#include <fost/tagged-string.hpp>


namespace fostlib {


    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const tagged_string< S, I > &l,
        const nullable< tagged_string< S, I > > &r
    ) {
        if ( not r ) return l;
        else return l + r.value();
    }
    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const tagged_string< S, I > &l,
        const tagged_string< S, I > &m,
        const nullable< tagged_string< S, I > > &r
    ) {
        if ( not r )
            return l;
        else
            return l + m + r.value();
    }
    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const nullable< tagged_string< S, I > > &l,
        const tagged_string< S, I > &m,
        const nullable< tagged_string< S, I > > &r
    ) {
        if ( not l ) return r;
        else if ( not r ) return l;
        else return l.value() + m + r.value();
    }
    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const nullable< tagged_string< S, I > > &l,
        const tagged_string< S, I > &m,
        const tagged_string< S, I > &r
    ) {
        if ( not l ) return r;
        else return l.value() + m + r;
    }


}


/*
    fostlib::nullable Implementation
*/


#include <fost/exception/null.hpp>


#endif // FOST_STRING_NULLABLE_HPP

