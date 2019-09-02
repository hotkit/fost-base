/**
    Copyright 2008-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/coerce.hpp>
#include <fost/exception.hpp>
#include <fost/filesystem.hpp>

#include <sstream>


namespace fostlib {


    class json;


    /// Convert a Boost error to JSON
    template<>
    struct coercer<json, fostlib::error_code> {
        json coerce(const fostlib::error_code &e) {
            return json(e.message().c_str());
        }
    };


    /// Coerce fostlib::exceptions::exception instances to a string
    template<typename E>
    struct coercer<
            string,
            E,
            std::enable_if_t<std::is_base_of_v<exceptions::exception, E>>> {
        string coerce(const E &e) {
            std::stringstream ss;
            e.printOn(ss);
            return string(ss.str());
        }
    };
    /// Coerce fostlib::exceptions::exception instances to json
    template<typename E>
    struct coercer<
            json,
            E,
            std::enable_if_t<std::is_base_of_v<exceptions::exception, E>>> {
        json coerce(const E &e) { return e.as_json(); }
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer<string, t_null> {
        string coerce(t_null);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<string, bool> {
        string coerce(bool b);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<uint16_t, f5::u8view> {
        uint16_t coerce(f5::u8view);
    };
    template<>
    struct coercer<uint16_t, f5::lstring> {
        auto coerce(f5::lstring s) {
            return fostlib::coercer<uint16_t, f5::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<uint16_t, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<uint16_t, f5::u8view>().coerce(s);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<int, f5::u8view> {
        int coerce(f5::u8view);
    };
    template<>
    struct coercer<int, f5::lstring> {
        auto coerce(f5::lstring s) {
            return fostlib::coercer<int, f5::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<int, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<int, f5::u8view>().coerce(s);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<int64_t, f5::u8view> {
        int64_t coerce(f5::u8view);
    };
    template<>
    struct coercer<int64_t, f5::lstring> {
        auto coerce(f5::lstring s) {
            return fostlib::coercer<int64_t, f5::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<int64_t, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<int64_t, f5::u8view>().coerce(s);
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, int64_t> {
        string coerce(int64_t i);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<string, uint64_t> {
        string coerce(uint64_t i);
    };

    template<typename I>
    struct coercer<string, I, std::enable_if_t<std::is_integral_v<I>>> {
        string coerce(I i) {
            return fostlib::coercer<string, int64_t>().coerce(i);
        }
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<double, f5::u8view> {
        double coerce(f5::u8view w);
    };
    template<>
    struct coercer<double, f5::lstring> {
        auto coerce(f5::lstring s) {
            return fostlib::coercer<double, f5::u8view>().coerce(s);
        }
    };
    template<>
    struct coercer<double, string> {
        auto coerce(const string &s) {
            return fostlib::coercer<double, f5::u8view>().coerce(s);
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, double> {
        string coerce(double d);
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer<string, nliteral> {
        string coerce(nliteral str) { return string(str); }
    };
    template<std::size_t L>
    struct FOST_CORE_DECLSPEC coercer<string, char[L]> {
        string coerce(const char str[L]) { return string(str); }
    };
    template<std::size_t L>
    struct FOST_CORE_DECLSPEC coercer<string, const char[L]> {
        string coerce(const char str[L]) { return string(str); }
    };
    template<>
    struct coercer<string, wliteral> {
        string coerce(wliteral str) { return string(str); }
    };
    template<std::size_t L>
    struct FOST_CORE_DECLSPEC coercer<string, wchar_t[L]> {
        string coerce(const wchar_t str[L]) { return string(str); }
    };
    template<std::size_t L>
    struct FOST_CORE_DECLSPEC coercer<string, const wchar_t[L]> {
        string coerce(const wchar_t str[L]) { return string(str); }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, std::vector<wchar_t>> {
        string coerce(const std::vector<wchar_t> &c);
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer<std::wstring, string> {
        std::wstring coerce(const string &str);
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, std::wstring> {
        string coerce(const std::wstring &str);
    };

    template<>
    struct coercer<string, float> {
        string coerce(float f) { return coercer<string, double>().coerce(f); }
    };


}
