/**
    Copyright 2018-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include <fost/base16.hpp>
#include <fost/exception/parse_error.hpp>


namespace {


    const char *b16chars = "0123456789ABCDEF";


}


void fostlib::base16_string_tag::do_encode(nliteral l, ascii_string &s) {
    s = l;
    check_encoded(s);
}


void fostlib::base16_string_tag::do_encode(
        const ascii_string &l, ascii_string &s) {
    s = l;
    check_encoded(s);
}


void fostlib::base16_string_tag::check_encoded(const fostlib::ascii_string &s) {
    if (s.underlying().find_first_not_of(b16chars) != std::string::npos)
        throw fostlib::exceptions::parse_error(
                "Non base16 character found in string", coerce<string>(s));
}


std::vector<unsigned char>
        fostlib::coercer<std::vector<unsigned char>, fostlib::base16_string>::
                coerce(const base16_string &s) {
    if (s.code_points() & 1) {
        throw fostlib::exceptions::parse_error(
                "Base16 data must contain whole bytes, "
                "i.e. an even number of characters");
    }
    std::vector<unsigned char> v;
    std::size_t p{4}, b{};
    for (auto c : s) {
        const auto ch = std::find(b16chars, b16chars + 16, c) - b16chars;
        b += ch << p;
        if (not p) {
            v.push_back(b);
            b = 0;
            p = 4;
        } else {
            p = 0;
        }
    }
    return v;
}
