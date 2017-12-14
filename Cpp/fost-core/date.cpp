/*
    Copyright 2000-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/date.hpp>
#include <fost/timediff.hpp>
#include <fost/unicode.hpp>


using namespace fostlib;


string fostlib::coercer<string, date>::coerce(date d) {
    return fostlib::coerce<string>(fostlib::coerce<ascii_string>(
        boost::gregorian::to_iso_extended_string(fostlib::coerce< boost::gregorian::date >(d))));
}


date fostlib::coercer<date, json>::coerce(const json &j) {
    auto s(j.get<f5::u8view>());
    if ( not s ) throw exceptions::not_implemented(__func__,  "With non-string JSON", j);
    return date(boost::gregorian::from_string(std::string(s.value())));
}

