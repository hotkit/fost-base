/**
    Copyright 2007-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/parse/json.hpp>

#include <fost/unicode.hpp>
#include <fost/exception/parse_error.hpp>


namespace {
    const fostlib::json_parser<
            f5::cord::const_u32u16_iterator<fostlib::string::const_iterator>>
            c_json_str_rule;
    const fostlib::json_parser<
            f5::cord::const_u32u16_iterator<f5::u8view::const_iterator>>
            c_json_u8v_rule;
    const fostlib::json_parser<f5::u16view::const_u16_iterator> c_json_u16_rule;
}


fostlib::json fostlib::json::parse(f5::u8view toparse) {
    fostlib::json ret{};
    auto pos = f5::cord::make_u32u16_iterator(toparse.begin(), toparse.end());
    try {
        if (boost::spirit::qi::parse(pos.first, pos.second, c_json_u8v_rule, ret)
            && pos.first == pos.second) {
            return ret;
        } else {
            throw exceptions::parse_error(
                    "Whilst parsing JSON string",
                    string(pos.first.u32_iterator(), pos.second.u32_iterator()));
        }
    } catch (std::runtime_error &e) {
        throw exceptions::parse_error(
                e.what(),
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}


fostlib::json fostlib::json::parse(f5::u16view s) {
    fostlib::json ret{};
    auto pos = s.u16begin();
    try {
        if (boost::spirit::qi::parse(pos, s.u16end(), c_json_u16_rule, ret)
            && pos == s.u16end()) {
            return ret;
        } else {
            throw exceptions::parse_error{
                    "Whilst parsing JSON string",
                    fostlib::coerce<f5::u8string>(f5::u16view(pos, s.u16end() - pos))};
        }
    } catch (std::runtime_error &e) {
        throw exceptions::parse_error{
                e.what(),
                fostlib::coerce<f5::u8string>(f5::u16view(pos, s.u16end() - pos))};
    }
}


fostlib::json fostlib::json::parse(const string &toparse, const json &def) {
    fostlib::json ret{};
    auto pos = f5::cord::make_u32u16_iterator(toparse.begin(), toparse.end());
    try {
        if (boost::spirit::qi::parse(pos.first, pos.second, c_json_str_rule, ret)
            && pos.first == pos.second) {
            return ret;
        } else {
            return def;
        }
    } catch (std::runtime_error &e) {
        throw exceptions::parse_error(
                e.what(),
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}
