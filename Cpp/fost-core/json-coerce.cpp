/**
    Copyright 2008-2019, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/insert.hpp>
#include <fost/push_back.hpp>
#include <fost/exception/cast_fault.hpp>
#include <fost/exception/json_error.hpp>
#include <fost/exception/not_a_number.hpp>


using namespace fostlib;


/*
    as_bool
*/
namespace {
    struct as_bool {
        bool operator()(std::monostate) const { return false; }
        bool operator()(bool b) const { return b; }
        bool operator()(int64_t i) const { return i; }
        bool operator()(double d) const { return d != 0.; }
        bool operator()(f5::lstring s) const { return not s.empty(); }
        bool operator()(const string &s) const {
            return not s.empty();
        }
        bool operator()(const json::array_p &a) const { return a->size(); }
        bool operator()(const json::object_p &o) const { return o->size(); }
    };
}
bool fostlib::coercer<bool, json>::coerce(const json &j) {
    return j.apply_visitor(::as_bool());
}


/*
    int64_t
*/
namespace {
    struct as_int {
        int64_t operator()(std::monostate) const {
            throw fostlib::exceptions::null("Cannot convert null to a number");
        }
        int64_t operator()(bool b) const { return b ? 1 : 0; }
        int64_t operator()(int64_t i) const { return i; }
        int64_t operator()(double d) const { return int64_t(d); }
        int64_t operator()(f5::lstring s) const { return coerce<int64_t>(s); }
        int64_t operator()(const string &s) const {
            return coerce<int64_t>(s);
        }
        int64_t operator()(const json::array_p &) const {
            throw fostlib::exceptions::not_a_number(
                    "Array cannot convert to a number");
        }
        int64_t operator()(const json::object_p &) const {
            throw fostlib::exceptions::not_a_number(
                    "Object cannot convert to a number");
        }
    };
}
int64_t fostlib::coercer<int64_t, json>::coerce(const json &j) {
    return j.apply_visitor(::as_int());
}


/*
    double
*/
namespace {
    struct as_double {
        double operator()(std::monostate) const {
            throw fostlib::exceptions::null(L"Cannot convert null to double");
        }
        double operator()(bool b) const { return b ? 1 : 0; }
        double operator()(int64_t i) const { return double(i); }
        double operator()(double d) const { return d; }
        double operator()(f5::lstring s) const { return coerce<double>(s); }
        double operator()(const string &s) const {
            return coerce<double>(s);
        }
        double operator()(const json::array_p &) const {
            throw fostlib::exceptions::not_a_number(
                    "Array cannot convert to a number");
        }
        double operator()(const json::object_p &) const {
            throw fostlib::exceptions::not_a_number(
                    "Object cannot convert to a number");
        }
    };
}
double fostlib::coercer<double, json>::coerce(const json &j) {
    return j.apply_visitor(::as_double());
}


/*
    strings
*/
namespace {
    struct as_u8view {
        f5::u8view operator()(std::monostate) const {
            throw fostlib::exceptions::null(
                    "Cannot convert null to f5::u8view");
        }
        f5::u8view operator()(bool) const {
            throw fostlib::exceptions::cast_fault(
                    "Cannot convert bool to f5::u8view");
        }
        f5::u8view operator()(int64_t) const {
            throw fostlib::exceptions::cast_fault(
                    "Cannot convert int64_t to f5::u8view");
        }
        f5::u8view operator()(double) const {
            throw fostlib::exceptions::cast_fault(
                    "Cannot convert double to f5::u8view");
        }
        f5::u8view operator()(f5::lstring s) const { return f5::u8view(s); }
        f5::u8view operator()(const string &s) const {
            return f5::u8view(s);
        }
        f5::u8view operator()(json::array_p const &a) const {
            fostlib::exceptions::cast_fault error(
                    "Cannot convert a JSON array to a string");
            fostlib::insert(error.data(), "array", *a);
            throw error;
        }
        f5::u8view operator()(json::object_p const &o) const {
            fostlib::exceptions::cast_fault error(
                    "Cannot convert a JSON object to a string");
            fostlib::insert(error.data(), "object", *o);
            throw error;
        }
    };
}
f5::u8view fostlib::coercer<f5::u8view, json>::coerce(const json &j) {
    return j.apply_visitor(::as_u8view());
}
namespace {
    struct as_nullable_u8view {
        nullable<f5::u8view> operator()(std::monostate) const { return null; }
        nullable<f5::u8view> operator()(bool) const { return null; }
        nullable<f5::u8view> operator()(int64_t) const { return null; }
        nullable<f5::u8view> operator()(double) const { return null; }
        nullable<f5::u8view> operator()(f5::lstring s) const {
            return f5::u8view(s);
        }
        nullable<f5::u8view> operator()(const string &s) const {
            return f5::u8view(s);
        }
        nullable<f5::u8view> operator()(const json::array_p &) const {
            return null;
        }
        nullable<f5::u8view> operator()(const json::object_p &) const {
            return null;
        }
    };
}
nullable<f5::u8view>
        fostlib::coercer<nullable<f5::u8view>, json>::coerce(const json &j) {
    return j.apply_visitor(::as_nullable_u8view());
}
namespace {
    struct as_string {
        string operator()(std::monostate) const {
            throw fostlib::exceptions::null("Cannot convert null to string");
        }
        string operator()(bool b) const { return coerce<string>(b); }
        string operator()(int64_t i) const { return coerce<string>(i); }
        string operator()(double d) const { return coerce<string>(d); }
        string operator()(f5::lstring s) const { return string(s); }
        string operator()(const string &s) const { return s; }
        string operator()(const json::array_p &a) const {
            fostlib::exceptions::cast_fault error(
                    "Cannot convert a JSON array to a string");
            fostlib::insert(error.data(), "array", *a);
            throw error;
        }
        string operator()(const json::object_p &o) const {
            fostlib::exceptions::cast_fault error(
                    "Cannot convert a JSON object to a string");
            fostlib::insert(error.data(), "object", *o);
            throw error;
        }
    };
}
string fostlib::coercer<string, json>::coerce(const json &j) {
    return j.apply_visitor(::as_string());
}


/*
    jcursor
*/

namespace {
    struct jc_as_js {
        json operator()(json::array_t::size_type p) const {
            return json(coerce<int64_t>(p));
        }
        json operator()(const string &p) const { return json(p); }
    };
}
json fostlib::coercer<json, jcursor>::coerce(const jcursor &j) {
    json::array_t cursor;
    for (jcursor::const_iterator p(j.begin()); p != j.end(); ++p)
        push_back(cursor, std::visit(::jc_as_js(), *p));
    return cursor;
}


jcursor fostlib::coercer<jcursor, json>::coerce(const json &j) {
    jcursor ret;
    if (j.isnull())
        return ret;
    else if (!j.isarray())
        throw exceptions::json_error(
                "The JSON must contain an array to convertible to a jcursor",
                j);
    for (std::size_t i(0); j.has_key(i); ++i) ret /= j[i];
    return ret;
}
