/**
    Copyright 2008-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <optional>
#include <ostream>


namespace fostlib {


    /// Alias `t_null` to the standard version.
    using t_null = std::nullopt_t;

    /// The null value
    constexpr auto const null = std::nullopt;


}


namespace std {
    /// We want the nulls to compare equal
    inline bool operator==(nullopt_t, nullopt_t) { return true; }
    inline bool operator!=(nullopt_t, nullopt_t) { return false; }

    /// Print nullopt_t values
    template<typename C>
    basic_ostream<C> &operator<<(basic_ostream<C> &o, nullopt_t) {
        return o << "** null **";
    }
}
