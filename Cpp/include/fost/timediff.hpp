/*
    Copyright 2012-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TIMEDIFF_HPP
#define FOST_TIMEDIFF_HPP
#pragma once


#include <fost/json.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


namespace fostlib {


    /// Use the boost provided type for now
    typedef boost::posix_time::time_duration timediff;


    /// Use the boost provided type for now
    using boost::posix_time::microseconds;
    /// Use the boost provided type for now
    using boost::posix_time::milliseconds;
    /// Use the boost provided type for now
    using boost::posix_time::seconds;
    /// Use the boost provided type for now
    using boost::posix_time::minutes;
    /// Use the boost provided type for now
    using boost::posix_time::hours;

    /// Allow us to deal with days directly in code
    typedef boost::gregorian::date_duration days;


    template<>
    struct FOST_CORE_DECLSPEC coercer<json, timediff> {
        json coerce(const timediff &);
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<timediff, json> {
        timediff coerce(const json &);
    };


}


namespace std {


    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const fostlib::timediff &td) {
        return o << boost::posix_time::to_simple_string(td);
    }


}


#endif // FOST_TIMEDIFF_HPP
