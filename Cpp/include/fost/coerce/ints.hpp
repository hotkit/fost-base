/**
    Copyright 2008-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_COERCE_INTS_HPP
#define FOST_COERCE_INTS_HPP
#pragma once


#include <fost/coerce.hpp>
#include <fost/exception/out_of_range.hpp>

#include <limits>
#include <type_traits>


namespace fostlib {


    /**
     * ## Coerce between integral types
    */
    template<typename T, typename F>
    struct coercer<T, F,
        std::enable_if_t<std::is_integral_v<T> && std::is_integral_v<F>>>
    {
        using limits_T = std::numeric_limits<T>;
        using limits_F = std::numeric_limits<F>;

        T coerce(F f) const {
            if constexpr ( std::is_same_v<T, bool> ) {
                return f ? true : false;
            } else {
                if constexpr ( limits_T::is_signed == limits_F::is_signed ) {
                    if constexpr ( sizeof(T) < sizeof(F) ) {
                        if ( f < limits_T::min() ) {
                            throw fostlib::exceptions::out_of_range<T, F>(
                                limits_T::min(), limits_T::max(), f);
                        }
                        if ( f > limits_T::max() ) {
                            throw fostlib::exceptions::out_of_range<T, F>(
                                limits_T::min(), limits_T::max(), f);
                        }
                    }
                } else if constexpr ( limits_T::is_signed ) {
                    if constexpr ( sizeof(T) <= sizeof(F) ) {
                        if ( f > limits_T::max() ) {
                            throw fostlib::exceptions::out_of_range<T, F>(
                                limits_T::min(), limits_T::max(), f);
                        }
                    }
                } else if constexpr ( limits_F::is_signed ) {
                    if ( f < 0 ) {
                        throw fostlib::exceptions::out_of_range<T, F>(
                            limits_T::min(), limits_T::max(), f);
                    }
                    if constexpr ( sizeof(T) <= sizeof(F) ) {
                        if ( f > limits_T::max() ) {
                            throw fostlib::exceptions::out_of_range<T, F>(
                                limits_T::min(), limits_T::max(), f);
                        }
                    }
                }
                return T(f);
            }
        }
    };


}


#endif // FOST_COERCE_INTS_HPP
