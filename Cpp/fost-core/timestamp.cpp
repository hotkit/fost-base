/**
    Copyright 2000-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/log.hpp>
#include <fost/timestamp.hpp>
#include <fost/exception/parse_error.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace fostlib;


string fostlib::coercer<string, std::chrono::system_clock::time_point>::coerce(
        std::chrono::system_clock::time_point const t) {
    auto const seconds = std::chrono::floor<std::chrono::seconds>(t);
    auto const sec_part = t - seconds;
    int64_t const ns =
            std::chrono::duration<uint64_t, std::nano>(sec_part).count();
    auto const tt = std::chrono::system_clock::to_time_t(t);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&tt), "%FT%T.");
    ss.width(9);
    ss.fill('0');
    ss << ns << "Z";

    return string{ss.str()};
}


string fostlib::coercer<string, timestamp>::coerce(timestamp t) {
    std::string s = boost::posix_time::to_iso_extended_string(
                            fostlib::coerce<boost::posix_time::ptime>(t))
            + "Z";
    return fostlib::coerce<string>(fostlib::coerce<ascii_string>(s));
}


timestamp fostlib::coercer<timestamp, string>::coerce(const string &s) {
    std::string repr;
    if (s.endswith("Z")) {
        repr = fostlib::coerce<ascii_string>(s.substr(0, s.length() - 1))
                       .underlying();
    } else {
        repr = fostlib::coerce<ascii_string>(s).underlying();
    }
    if (repr.length() > 10) { repr[10] = ' '; }
    try {
        if (repr.length() <= 10)
            return timestamp(boost::posix_time::ptime(
                    boost::gregorian::from_simple_string(repr)));
        else
            return timestamp(boost::posix_time::time_from_string(repr));
    } catch (boost::bad_lexical_cast &) {
        exceptions::parse_error e("Parsing a timestamp from a string");
        insert(e.data(),
               "fostlib::coercer< timestamp, string >::coerce( const string &s "
               ")",
               "repr", fostlib::coerce<string>(utf8_string(repr)));
        insert(e.data(),
               "fostlib::coercer< timestamp, string >::coerce( const string &s "
               ")",
               "repr.length()", repr.length());
        throw e;
    }
}
