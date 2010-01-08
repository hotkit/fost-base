/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COERCE_INTS_HPP
#define FOST_COERCE_INTS_HPP
#pragma once


#include <fost/coerce.hpp>
#include <fost/exception/out_of_range.hpp>


namespace fostlib {


    template< typename I >
    struct coercer<
        int64_t, I,
        typename boost::enable_if< boost::is_integral< I > >::type
    > {
        int64_t coerce( I i ) {
            return int64_t( i );
        }
    };
    template< typename I >
    struct coercer<
        I, int64_t,
        typename boost::enable_if< boost::is_integral< I > >::type
    > {
        I coerce( int64_t i ) {
            if (
                i > std::numeric_limits< I >::max()
                || i < std::numeric_limits< I >::min()
            )
                throw fostlib::exceptions::out_of_range< int64_t >(
                    std::numeric_limits< I >::min(),
                    std::numeric_limits< I >::max(),
                    i
                );
            return I( i );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< uint64_t, int64_t > {
        uint64_t coerce( int64_t i );
    };


}


#endif // FOST_COERCE_INTS_HPP
