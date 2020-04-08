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


    /// Can be used to detect when a type conversion can be done through strict
    /// construction.
    template<typename, typename, typename = std::void_t<>>
    struct is_brace_constructible : std::false_type {};
    template<typename T, typename F>
    struct is_brace_constructible<
            T,
            F,
            std::void_t<decltype(T{std::declval<F>()})>> : std::true_type {};

    template<typename T, typename F>
    inline constexpr bool is_brace_constructible_v =
            is_brace_constructible<T, F>::value;


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
        static_assert(is_brace_constructible_v<T, F>);

        T coerce(F const &f) { return T{f}; }
        T coerce(F &&f) { return T{std::move(f)}; }
    };


    template<typename T, typename F>
    inline T coerce(F &&f) {
        return coercer<T, std::decay_t<F>>{}.coerce(std::forward<F>(f));
    }


}


#endif // FOST_COERCE_HPP
