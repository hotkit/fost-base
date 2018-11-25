/*
    Copyright 2000-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PERIOD_HPP
#define FOST_PERIOD_HPP
#pragma once


#include <fost/datetime>


namespace fostlib {


    /// Allows us to store a controlled value which is only valid between two
    /// timestamps
    template<typename V>
    class period;


    /// Specialisation of period for where there is no controlled value
    template<>
    class period<void> {
      public:
        /// The start of the period. Null is forever
        accessors<nullable<timestamp>> start;
        /// The end of the period. Null is forever
        accessors<nullable<timestamp>> end;

        /// Allow periods to be compared for equality
        bool operator==(const period<void> &p) const {
            return start() == p.start() && end() == p.end();
        }
        /// Allow periods to be compared for inequality
        bool operator!=(const period<void> &p) const { return !(*this == p); }

        /// Construct an empty period that lasts forever
        period() {}
        /// Construct a period from two timestamps
        period(const timestamp &start, const timestamp &end)
        : start(start), end(end) {}
    };


    /// Specialisation of period for where there is a controlled value
    template<typename V>
    class period : private period<void> {
      public:
        using period<void>::start;
        using period<void>::end;

        /// The value
        accessors<V> value;

        /// Allow periods to be compared for equality
        bool operator==(const period &p) const {
            return this->period<void>::operator==(p) && value() == p.value();
        }
        /// Allow periods to be compared for inequality
        bool operator!=(const period &p) const { return !(*this == p); }
    };


}


namespace std {


    template<typename V>
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const fostlib::period<V> &p) {
        return o << fostlib::coerce<fostlib::string>(p);
    }


}


#endif // FOST_PERIOD_HPP
