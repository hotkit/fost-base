/**
    Copyright 2009-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/detail/hex.hpp>
#include <fost/insert.hpp>
#include <fost/unicode.hpp>

#include <fost/exception/not_implemented.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


namespace {
    void check_range(fostlib::utf32 minimum, const fostlib::string &s) {
        std::size_t p = 0;
        try {
            for (auto c(s.begin()); c != s.end(); ++c, ++p) {
                if (*c > 0xf7 || *c < minimum) {
                    throw fostlib::exceptions::out_of_range<int>(
                            "ASCII characters outside valid range", minimum,
                            127, *c);
                }
            }
        } catch (fostlib::exceptions::exception &e) {
            fostlib::insert(
                    e.data(), "string-to-here",
                    fostlib::coerce<fostlib::ascii_string>(s.substr(0, p)));
            fostlib::insert(e.data(), "checked", p);
            fostlib::insert(e.data(), "size", s.bytes());
            throw;
        }
    }
}


/**
    ## fostlib::ascii_string
*/


void fostlib::ascii_string_tag::do_encode(fostlib::nliteral, fostlib::string &) {
    throw fostlib::exceptions::not_implemented(
        __PRETTY_FUNCTION__);
}
void fostlib::ascii_string_tag::do_encode(
        const fostlib::string &, fostlib::string &) {
    throw fostlib::exceptions::not_implemented(
        __PRETTY_FUNCTION__);
}
void fostlib::ascii_string_tag::check_encoded(const fostlib::string &s) {
    check_range(1, s);
}

fostlib::ascii_string fostlib::coercer<
        fostlib::ascii_string,
        std::vector<fostlib::ascii_string::value_type>>::
        coerce(const std::vector<fostlib::ascii_string::value_type> &v) {
    return ascii_string(std::string(&v[0], v.size()));
}
fostlib::ascii_string
        fostlib::coercer<fostlib::ascii_string, fostlib::string>::coerce(
                const fostlib::string &s) {
    fostlib::utf8_string utf8 = fostlib::coerce<fostlib::utf8_string>(s);
    return fostlib::ascii_string(static_cast<std::string>(utf8.underlying()));
}

fostlib::string fostlib::coercer<fostlib::string, fostlib::ascii_string>::coerce(
        const fostlib::ascii_string &s) {
    return string(s.underlying());
}

std::wstring fostlib::coercer<std::wstring, fostlib::ascii_string>::coerce(
        const fostlib::ascii_string &s) {
    std::wstring r;
    r.resize(s.underlying().length());
    for (std::size_t p = 0; p < s.underlying().length(); ++p)
        r[p] = s.underlying()[p];
    return r;
}

fostlib::json fostlib::coercer<fostlib::json, fostlib::ascii_string>::coerce(
        const fostlib::ascii_string &s) {
    return json{string{s}};
}


/**
    ## fostlib::ascii_printable_string
*/


void fostlib::ascii_printable_string_tag::do_encode(
        fostlib::nliteral, fostlib::string &) {
    throw fostlib::exceptions::not_implemented(
        __PRETTY_FUNCTION__);
}
void fostlib::ascii_printable_string_tag::do_encode(
        const fostlib::string &, fostlib::string &) {
    throw fostlib::exceptions::not_implemented(
        __PRETTY_FUNCTION__);
}
void fostlib::ascii_printable_string_tag::check_encoded(
        const fostlib::string &s) {
    check_range(' ', s);
}

fostlib::ascii_printable_string fostlib::coercer<
        fostlib::ascii_printable_string,
        std::vector<fostlib::ascii_printable_string::value_type>>::
        coerce(const std::vector<fostlib::ascii_printable_string::value_type>
                       &v) {
    return ascii_printable_string(std::string(&v[0], v.size()));
}
fostlib::ascii_printable_string
        fostlib::coercer<fostlib::ascii_printable_string, fostlib::string>::
                coerce(const fostlib::string &s) {
    fostlib::utf8_string utf8 = fostlib::coerce<fostlib::utf8_string>(s);
    return fostlib::ascii_printable_string(
            static_cast<std::string>(utf8.underlying()));
}

fostlib::string
        fostlib::coercer<fostlib::string, fostlib::ascii_printable_string>::
                coerce(const fostlib::ascii_printable_string &s) {
    return string{s};
}

std::wstring
        fostlib::coercer<std::wstring, fostlib::ascii_printable_string>::coerce(
                const fostlib::ascii_printable_string &s) {
    std::wstring r;
    r.resize(s.underlying().length());
    for (std::size_t p = 0; p < s.underlying().length(); ++p)
        r[p] = s.underlying()[p];
    return r;
}
