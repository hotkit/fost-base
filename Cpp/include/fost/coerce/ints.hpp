/*
    Copyright 2008-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COERCE_INTS_HPP
#define FOST_COERCE_INTS_HPP
#pragma once


#include <fost/coerce.hpp>
#include <fost/exception/out_of_range.hpp>

#include <boost/type_traits.hpp>


namespace fostlib {


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


    /// If the two types are integral, the from type is unsigned and the from type is larger than the to type we are safe
    template< typename T, typename F >
    struct coercer<
        T, F,
        typename boost::enable_if<
            boost::mpl::and_<
                boost::mpl::bool_< boost::is_integral< T >::value >,
                boost::mpl::bool_< boost::is_integral< F >::value >,
                boost::mpl::bool_< boost::is_unsigned< F >::value >,
                boost::mpl::less<
                    boost::mpl::int_< sizeof(F) >,
                    boost::mpl::int_< sizeof(T) >
                >
            >
        >::type
    > {
        T coerce( F i ) { return i; }
    };


}


#endif // FOST_COERCE_INTS_HPP
