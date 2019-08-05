/**
    Copyright 2007-2019, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/parse/json.hpp>

#include <fost/unicode.hpp>
#include <fost/exception/parse_error.hpp>


namespace {
    const fostlib::sloppy_json_parser<
            f5::cord::const_u32u16_iterator<fostlib::string::const_iterator>>
            c_json_str_rule;
    const fostlib::sloppy_json_parser<
            f5::cord::const_u32u16_iterator<f5::u8view::const_iterator>>
            c_json_u8v_rule;
}


fostlib::json fostlib::json::sloppy_parse(f5::u8view toparse) {
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


fostlib::json fostlib::json::sloppy_parse(const string &toparse, const json &def) {
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
