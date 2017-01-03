/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/parse/json.hpp>

#include <fost/unicode.hpp>
#include <fost/exception/parse_error.hpp>


namespace {
    const fostlib::json_parser<
        f5::const_u32u16_iterator<
            fostlib::string::const_iterator>> c_json_str_rule;
    const fostlib::json_parser<
        f5::const_u32u16_iterator<
            fostlib::utf::u8_view::const_iterator>> c_json_u8v_rule;
}


fostlib::json fostlib::json::parse(const string &toparse) {
    fostlib::json ret{};
    fostlib::parser_lock lock;
    auto pos = f5::make_u32u16_iterator(toparse.begin(), toparse.end());
    try {
        if ( boost::spirit::qi::parse(pos.first, pos.second, c_json_str_rule, ret) && pos.first == pos.second ) {
            return ret;
        } else {
            throw exceptions::parse_error("Whilst parsing JSON string",
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
        }
    } catch ( std::runtime_error &e ) {
        throw exceptions::parse_error(e.what(),
            string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}


fostlib::json fostlib::json::parse(array_view<unsigned char> buffer) {
    fostlib::json ret{};
    fostlib::parser_lock lock;
    fostlib::utf::u8_view u8v(buffer);
    auto pos = f5::make_u32u16_iterator(u8v.begin(), u8v.end());
    try {
        if ( boost::spirit::qi::parse(pos.first, pos.second, c_json_u8v_rule, ret) && pos.first == pos.second ) {
            return ret;
        } else {
            throw exceptions::parse_error("Whilst parsing JSON string",
                string(pos.first.u32_iterator(), pos.second.u32_iterator()));
        }
    } catch ( std::runtime_error &e ) {
        throw exceptions::parse_error(e.what(),
            string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}


fostlib::json fostlib::json::parse(const string &toparse, const json &def) {
    fostlib::json ret{};
    fostlib::parser_lock lock;
    auto pos = f5::make_u32u16_iterator(toparse.begin(), toparse.end());
    try {
        if ( boost::spirit::qi::parse(pos.first, pos.second, c_json_str_rule, ret) && pos.first == pos.second ) {
            return ret;
        } else {
            return def;
        }
    } catch ( std::runtime_error &e ) {
        throw exceptions::parse_error(e.what(),
            string(pos.first.u32_iterator(), pos.second.u32_iterator()));
    }
}

