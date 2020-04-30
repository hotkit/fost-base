/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_COERCE_NULLABLE_HPP
#define FOST_COERCE_NULLABLE_HPP
#pragma once


#include <fost/nullable.hpp>
#include <fost/coerce.hpp>
#include <fost/exception/null.hpp>


namespace fostlib {


    template<typename T, typename F>
    struct coercer<T, std::optional<F>> {
        template<typename A>
        auto coerce(A &&f) {
            if (not f) {
                throw exceptions::null{__PRETTY_FUNCTION__};
            } else {
                return fostlib::coerce<T>(*std::forward<A>(f));
            }
        }
    };
    template<typename T, typename F>
    struct coercer<std::optional<T>, F> {
        template<typename A>
        std::optional<T> coerce(A &&f) {
            return fostlib::coerce<T>(std::forward<A>(f));
        }
    };
    template<typename T, typename F>
    struct coercer<std::optional<T>, std::optional<F>> {
        template<typename A>
        std::optional<T> coerce(A &&f) {
            if (not f) {
                return std::nullopt;
            } else {
                return fostlib::coerce<T>(*std::forward<A>(f));
            }
        }
    };


}


#endif // FOST_COERCE_NULLABLE_HPP
