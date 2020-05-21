/**
    Copyright 2007-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/pointers>
#include <fost/parse/parse.hpp>

#include <fost/unicode.hpp>

#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;


namespace {
    template<typename T>
    T unsigned_p(f5::u8view s) {
        T ret{};
        auto pos = s.begin();
        if (boost::spirit::qi::parse(
                    pos, s.end(), boost::spirit::qi::uint_parser<T>(), ret)
            && pos == s.end()) {
            return ret;
        } else {
            throw fostlib::exceptions::parse_error(
                    "Could not parse unsigned integer", s);
        }
    }
    template<typename T>
    T signed_p(f5::u8view s) {
        T ret{};
        auto pos = s.begin();
        if (boost::spirit::qi::parse(
                    pos, s.end(), boost::spirit::qi::int_parser<T>(), ret)
            && pos == s.end()) {
            return ret;
        } else {
            throw fostlib::exceptions::parse_error(
                    "Could not parse signed integer", s);
        }
    }
}


/**
    uint16_t
*/


uint16_t fostlib::coercer<uint16_t, f5::u8view>::coerce(f5::u8view s) {
    return unsigned_p<uint16_t>(s);
}


/**
    int
*/


int fostlib::coercer<int, f5::u8view>::coerce(f5::u8view s) {
    return signed_p<int>(s);
}


/**
    int64_t
*/


int64_t fostlib::coercer<int64_t, f5::u8view>::coerce(f5::u8view s) {
    return signed_p<int64_t>(s);
}


/**
    double
*/


double fostlib::coercer<double, f5::u8view>::coerce(f5::u8view s) {
    double ret{};
    auto pos = s.begin(), end = s.end();
    if (boost::spirit::qi::phrase_parse(
                pos, end, boost::spirit::qi::double_, boost::spirit::qi::space,
                ret)
        && pos == end) {
        return ret;
    } else {
        throw fostlib::exceptions::parse_error("Could not parse double", s);
    }
}


/**
    string
*/


string fostlib::coercer<string, t_null>::coerce(t_null) { return string(); }
string fostlib::coercer<string, bool>::coerce(bool b) {
    return b ? "true" : "false";
}


#ifdef FOST_OS_WINDOWS
#include "coerce-win.cpp"
#else
#include "coerce-linux.cpp"
#endif
