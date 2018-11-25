/*
    Copyright 2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/timediff.hpp>
#include <fost/insert.hpp>


fostlib::json fostlib::coercer<fostlib::json, fostlib::timediff>::coerce(
        const fostlib::timediff &td) {
    fostlib::json j;
    if (td.hours()) fostlib::insert(j, "hours", td.hours());
    if (td.minutes()) fostlib::insert(j, "minutes", td.minutes());
    if (td.seconds() || td.fractional_seconds()) {
        double seconds = td.seconds();
        seconds += td.fractional_seconds() / 1e6;
        fostlib::insert(j, "seconds", seconds);
    }
    return j;
}


fostlib::timediff fostlib::coercer<fostlib::timediff, fostlib::json>::coerce(
        const fostlib::json &js) {
    fostlib::timediff td(0, 0, 0);
    if (js.has_key("hours"))
        // An MSVC bug means we can't do the range check on the coercion
        td += fostlib::hours(long(fostlib::coerce<int64_t>(js["hours"])));
    if (js.has_key("minutes"))
        // An MSVC bug means we can't do the range check on the coercion
        td += fostlib::minutes(long(fostlib::coerce<int64_t>(js["minutes"])));
    if (js.has_key("seconds")) {
        double seconds = fostlib::coerce<double>(js["seconds"]), whole;
        double fraction = std::modf(seconds, &whole);
        td += fostlib::seconds(long(whole));
        td += fostlib::microseconds(int64_t(fraction * 1e6));
    }
    return td;
}
