/**
    Copyright 2000-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/insert.hpp>
#include <fost/timestamp.hpp>
#include <fost/parse/timestamp.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;


/**
    ## `fostlib::rfc1123_timestamp`
*/


void fostlib::rfc1123_timestamp_tag::do_encode(
        fostlib::nliteral, ascii_string &) {
    throw exceptions::not_implemented(
            "fostlib::rfc1123_timestamp_tag::do_encode( fostlib::nliteral "
            "from, ascii_string &into )");
}


void fostlib::rfc1123_timestamp_tag::do_encode(
        const ascii_string &, ascii_string &) {
    throw exceptions::not_implemented(
            "fostlib::rfc1123_timestamp_tag::do_encode( const ascii_string "
            "&from, ascii_string &into )");
}


void fostlib::rfc1123_timestamp_tag::check_encoded(const ascii_string &s) {
    try {
        if (s.underlying().length() != 31)
            throw exceptions::out_of_range<int>(
                    "RFC 1123 date format must be 31 characters", 31, 31,
                    s.underlying().length());
    } catch (exceptions::exception &e) {
        fostlib::insert(e.data(), "string", s);
        throw;
    }
}


rfc1123_timestamp
        fostlib::coercer<rfc1123_timestamp, timestamp>::coerce(timestamp t) {
    boost::posix_time::ptime ts = fostlib::coerce<boost::posix_time::ptime>(t);
    std::stringstream ss;
    ss.imbue(std::locale(
            ss.getloc(),
            new boost::posix_time::time_facet("%a, %d %b %Y %H:%M:%S +0000")));
    ss << ts;
    return rfc1123_timestamp(ascii_string(ss.str()));
}


timestamp fostlib::coercer<timestamp, rfc1123_timestamp>::coerce(
        const rfc1123_timestamp &ts) {
    timestamp ret{};
    auto pos = ts.underlying().underlying().begin();
    const auto end = ts.underlying().underlying().end();
    rfc1123_timestamp_parser<decltype(pos)> rule;
    if (boost::spirit::qi::phrase_parse(
                pos, end, rule, boost::spirit::qi::space, ret)
        && pos == end) {
        return ret;
    } else {
        throw exceptions::parse_error(
                "Parsing an RFC1123 time stamp", string(pos, end));
    }
}
