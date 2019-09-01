/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_NULLABLE_HPP
#define FOST_NULLABLE_HPP
#pragma once


#include <fost/nullable-core.hpp>


namespace std {
    /// Specialisation to allow fostlib::nullables to be used as keys in sets
    /// and maps.
    template<typename Y>
    struct less<fostlib::nullable<Y>> {
        bool operator()(
                const fostlib::nullable<Y> &l,
                const fostlib::nullable<Y> &r) const {
            if (l.isnull())
                return !r.isnull();
            else if (r.isnull())
                return false;
            else
                return std::less<Y>()(l.value(), r.value());
        }
    };
}


#ifdef FOST_STRING_HPP
#include <fost/detail/nullable.hpp>
#endif


#include <fost/exception/null.hpp>


#endif // FOST_NULLABLE_HPP
