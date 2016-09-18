/*
    Copyright 2007-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/parse/json.hpp>

#include <fost/unicode.hpp>
#include <fost/exception/parse_error.hpp>


fostlib::json fostlib::json::parse(const string &toparse) {
    fostlib::json ret;
    fostlib::parser_lock lock;
    const auto res = fostlib::parse(lock,
            coerce<std::wstring>(toparse).c_str(), json_p[phoenix::var(ret) = phoenix::arg1]);
    if ( not res.full ) throw exceptions::parse_error("Whilst parsing JSON string", toparse);
    return ret;
}


fostlib::json fostlib::json::parse(array_view<unsigned char> buffer) {
    /// What we really want to be able to do is to parse the UTF32 stream
    /// coming straight out of the buffer. The problem is that the \u
    /// escape sequences (which are UTF-16) make this very hard to do without
    /// making mistakes. Instead we're going to end up turning this into a
    /// UTF-16 buffer and then parse that. Sigh
    const auto u8 = utf::u8_view(buffer);
    const auto str = string(u8.begin(), u8.end());
    const auto prs = coerce<std::wstring>(str);
    json ret;
    fostlib::parser_lock lock;
    const auto res = fostlib::parse(lock, prs.c_str(), json_p[phoenix::var(ret) = phoenix::arg1]);
    if ( not res.full ) throw fostlib::exceptions::parse_error( L"Whilst parsing JSON string", str );
    return ret;
}


fostlib::json fostlib::json::parse(const string &toparse, const json &def) {
    fostlib::json ret;
    fostlib::parser_lock lock;
    const auto res = fostlib::parse(lock,
            coerce<std::wstring>(toparse).c_str(), json_p[phoenix::var(ret) = phoenix::arg1]);
    if ( not res.full ) return def;
    return ret;
}

