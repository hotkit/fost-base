/**
    Copyright 2001-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_NULLABLE_CORE_HPP
#define FOST_NULLABLE_CORE_HPP
#pragma once


#include <fost/null.hpp>


namespace fostlib {


    namespace detail {


        [[noreturn]] void throw_null_exception();


    }


    template<typename T>
    using nullable = std::optional<T>;


}


/// Allow nullable values to be printed
namespace std {
    template<typename C, typename Y>
    basic_ostream<C> &operator << (basic_ostream<C> &o, const optional<Y> &y) {
        return y ? o << y.value() : o << std::nullopt;
    }
}


#endif // FOST_NULLABLE_HPP

