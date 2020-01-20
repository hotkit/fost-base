/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_COERCE_HPP
#define FOST_COERCE_HPP
#pragma once


#include <fost/null.hpp>

#include <type_traits>


namespace fostlib {


    /// ## Coercion helper
    /**
     * This struct can be partially specialised to handle whole classes of
     * coercions
     */
    template<typename T, typename F, typename Enable = void>
    struct coercer {
        /**
         * If the `T` is constructible from the `F` then just forward to the
         * constructor.
         */
        std::enable_if_t<std::is_constructible_v<T, F const &>, T>
                coerce(F const &f) {
            return T{f};
        }
        std::enable_if_t<std::is_constructible_v<T, F>, T> coerce(F &&f) {
            return T{std::move(f)};
        }
    };


    template<typename T, typename F>
    T coerce(const F &f) {
        return coercer<T, F>().coerce(f);
    }


    /**
     * Cannot coerce from null to anything.
     */
    template<typename T>
    struct coercer<T, t_null> {};


}


#endif // FOST_COERCE_HPP
