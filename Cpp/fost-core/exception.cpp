/**
    Copyright 2001-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <fost/insert.hpp>
#include <fost/push_back.hpp>


namespace {
    fostlib::performance p_absorbed(fostlib::c_fost, "exceptions", "absorbed");
}


void fostlib::absorb_exception() noexcept {
    // An exception is in the process of being thrown away.
    // We want to be very careful not to do anything that may throw again.
    ++p_absorbed;
}


void fostlib::detail::throw_null_exception() {
    throw exceptions::null("Empty nullable instance");
}


/*
    fostlib::exceptions::exception
*/


fostlib::exceptions::exception::exception(const exception &e) noexcept {
    try {
        m_data = e.data();
    } catch (...) { absorb_exception(); }
}


fostlib::exceptions::exception::exception() noexcept {}


fostlib::exceptions::exception::exception(const fostlib::string &m) noexcept {
    try {
        insert(m_data, "message", m);
    } catch (...) { absorb_exception(); }
}


fostlib::exceptions::exception::~exception() noexcept try {
} catch (...) { absorb_exception(); }


const fostlib::json &fostlib::exceptions::exception::data() const {
    return m_data;
}


fostlib::json &fostlib::exceptions::exception::data() { return m_data; }


const char *fostlib::exceptions::exception::what() const noexcept {
    try {
        fostlib::stringstream ss;
        ss << string(message());
        string const text{ss.str()};
        m_what_string.reset(new char[text.bytes() + 1]{});
        std::copy(text.memory().begin(), text.memory().end(), m_what_string.get());
        return m_what_string.get();
    } catch (...) {
        return "Unknown fostlib::exceptions::exception. "
               "Exception throw during generation of exception description";
    }
}


fostlib::ostream &
        fostlib::exceptions::exception::printOn(fostlib::ostream &o) const {
    return o << string(message()) << "\nData: " << data();
}


fostlib::json fostlib::exceptions::exception::as_json() const {
    json out;
    insert(out, "exception", message());
    insert(out, "data", data());
    return out;
}


/**
    ## Individual exceptions
*/


fostlib::exceptions::ctrl_break::ctrl_break() {}


fostlib::exceptions::forwarded::forwarded(const json &repr) : exception() {
    try {
        insert(m_data, "exception", repr);
    } catch (...) { absorb_exception(); }
}
fostlib::wliteral const fostlib::exceptions::forwarded::message() const
        noexcept {
    return L"Forwarded exception";
}


#include <fost/exception/cast_fault.hpp>
fostlib::exceptions::cast_fault::cast_fault(const string &message) noexcept
: exception(message) {}
fostlib::wliteral const fostlib::exceptions::cast_fault::message() const
        noexcept {
    return L"The requested cast is not possible";
}


#include <fost/exception/external_process_failure.hpp>
fostlib::exceptions::external_process_failure::external_process_failure() noexcept
: exception() {}
fostlib::exceptions::external_process_failure::external_process_failure(
        const string &command) noexcept
: exception(command) {}
const wchar_t *const
        fostlib::exceptions::external_process_failure::message() const
        noexcept {
    return L"External process exit code not zero";
}


#include <fost/exception/file_error.hpp>
fostlib::exceptions::file_error::file_error(
        const string &message, const string &filename) noexcept
: exception(message) {
    try {
        insert(data(), "filename", filename);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::file_error::file_error(
        const string &message,
        const boost::filesystem::path &file,
        boost::system::error_code error) noexcept
: exception(message) {
    try {
        insert(data(), "filename", file.string().c_str());
        insert(data(), "error", error);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::file_error::message() const noexcept {
    return L"A File I/O error occured";
}


#include <fost/exception/json_error.hpp>
fostlib::exceptions::json_error::json_error(const string &message) noexcept
: exception(message) {}
fostlib::exceptions::json_error::json_error(
        const string &message, const json &value) noexcept
: exception(message) {
    try {
        insert(data(), "value", value);
    } catch (...) { fostlib::absorb_exception(); }
}
fostlib::wliteral const fostlib::exceptions::json_error::message() const
        noexcept {
    return L"JSON error";
}


#include <fost/exception/missing_setting.hpp>
fostlib::exceptions::missing_setting::missing_setting(
        const string &section, const string &name) noexcept
: exception() {
    try {
        insert(data(), "section", section);
        insert(data(), "name", name);
    } catch (...) { fostlib::absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::missing_setting::message() const
        noexcept {
    return L"Setting is missing";
}


#include <fost/exception/not_a_number.hpp>
fostlib::exceptions::not_a_number::not_a_number(const string &message) noexcept
: exception(message) {}
fostlib::exceptions::not_a_number::not_a_number(
        const string &message, const string &value) noexcept
: exception(message) {
    try {
        insert(data(), "value", value);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::not_a_number::message() const
        noexcept {
    return L"Not a number";
}


#include <fost/exception/not_implemented.hpp>
fostlib::exceptions::not_implemented::not_implemented() noexcept
: exception() {}
fostlib::exceptions::not_implemented::not_implemented(const string &func) noexcept
: exception() {
    try {
        insert(data(), "function", func);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::not_implemented::not_implemented(
        const string &func, const string &mess) noexcept
: exception(mess) {
    try {
        insert(data(), "function", func);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::not_implemented::not_implemented(
        const string &func, const string &mes, const string &extra) noexcept
: exception(mes) {
    try {
        insert(data(), "function", func);
        insert(data(), "detail", extra);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::not_implemented::not_implemented(
        const string &func, const string &mes, const json &extra)
: exception(mes) {
    insert(data(), "function", func);
    insert(data(), "detail", extra);
}
fostlib::exceptions::not_implemented::not_implemented(
        const string &func, boost::system::error_code error) noexcept
: exception() {
    try {
        insert(data(), "function", func);
        insert(data(), "error", error);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::not_implemented::not_implemented(
        const string &func,
        boost::system::error_code error,
        const string &mess) noexcept
: exception(mess) {
    try {
        insert(data(), "function", func);
        insert(data(), "error", error);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::not_implemented::message() const
        noexcept {
    return L"Feature not implemented";
}


#include <fost/exception/not_null.hpp>
fostlib::exceptions::not_null::not_null() noexcept {}
fostlib::exceptions::not_null::not_null(const string &mes) noexcept
: exception(mes) {}
fostlib::exceptions::not_null::not_null(
        const string &mes, const string &inf) noexcept
: exception(mes) {
    try {
        insert(data(), "info", inf);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::not_null::message() const noexcept {
    return L"A value was found where none was expected";
}


#include <fost/exception/not_unique.hpp>
fostlib::exceptions::not_unique::not_unique(const string &message) noexcept
: exception(message) {}
fostlib::exceptions::not_unique::not_unique(
        const string &error,
        const string &context,
        const string &alternative1,
        const string &alternative2) noexcept
: exception(error) {
    try {
        insert(data(), "context", context);
        push_back(data(), "alternatives", alternative1);
        push_back(data(), "alternatives", alternative2);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::not_unique::message() const noexcept {
    return L"Uniqueness violation";
}


#include <fost/exception/null.hpp>
fostlib::exceptions::null::null() noexcept {}
fostlib::exceptions::null::null(const string &mes) noexcept : exception(mes) {}
fostlib::exceptions::null::null(const string &mes, const string &inf) noexcept
: exception(mes) {
    try {
        insert(data(), "info", inf);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::null::message() const noexcept {
    return L"Attempt to de-reference null value";
}


#include <fost/exception/out_of_range.hpp>
fostlib::exceptions::out_of_range_string::out_of_range_string(
        const string &mn, const string &mx, const string &v) noexcept {
    try {
        insert(data(), "value", v);
        insert(data(), "min", mn);
        insert(data(), "max", mx);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::out_of_range_string::out_of_range_string(
        const string &mes,
        const string &mn,
        const string &mx,
        const string &v) noexcept
: exception(mes) {
    try {
        insert(data(), "value", v);
        insert(data(), "min", mn);
        insert(data(), "max", mx);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::out_of_range_string::message() const
        noexcept {
    return L"Value outside of allowable range";
}


#include <fost/exception/overflow.hpp>
fostlib::exceptions::overflow<fostlib::string>::overflow(
        const string &message) noexcept
: exception(message) {}
fostlib::exceptions::overflow<fostlib::string>::overflow(
        const string &message,
        const string &n,
        const string &d,
        const string &m) noexcept
: exception(message) {
    try {
        insert(data(), "numerator", n);
        insert(data(), "denominator", d);
        insert(data(), "multiplier", m);
    } catch (...) { absorb_exception(); }
}
fostlib::exceptions::overflow<fostlib::string>::overflow(
        const string &n, const string &d, const string &m) noexcept
: exception() {
    try {
        insert(data(), "numerator", n);
        insert(data(), "denominator", d);
        insert(data(), "multiplier", m);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const
        fostlib::exceptions::overflow<fostlib::string>::message() const
        noexcept {
    return L"An Overflow occured";
}


#include <fost/exception/parse_error.hpp>
fostlib::exceptions::parse_error::parse_error(const string &message) noexcept
: exception(message) {}
fostlib::exceptions::parse_error::parse_error(
        const string &message, const string &value) noexcept
: exception(message) {
    try {
        insert(data(), "value", value);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::parse_error::message() const
        noexcept {
    return L"Parsing error";
}


#include <fost/exception/settings_fault.hpp>
fostlib::exceptions::settings_fault::settings_fault(
        const string &error,
        const string &domain,
        const string &section,
        const string &name,
        const string &value) noexcept
: exception(error) {
    insert(data(), "domain", domain);
    insert(data(), "section", section);
    insert(data(), "name", name);
    insert(data(), "value", value);
}
const wchar_t *const fostlib::exceptions::settings_fault::message() const
        noexcept {
    return L"Internal Setting library fault";
}


#include <fost/exception/unexpected_eof.hpp>
fostlib::exceptions::unexpected_eof::unexpected_eof() noexcept : exception() {}
fostlib::exceptions::unexpected_eof::unexpected_eof(const string &msg) noexcept
: exception(msg) {}
fostlib::exceptions::unexpected_eof::unexpected_eof(
        const string &msg, const string &f) noexcept
: exception(msg) {
    try {
        insert(data(), "filename", f);
    } catch (...) { fostlib::absorb_exception(); }
}
fostlib::exceptions::unexpected_eof::unexpected_eof(
        const string &msg, boost::system::error_code error) noexcept
: exception(msg) {
    try {
        insert(data(), "error", error);
    } catch (...) { absorb_exception(); }
}
const wchar_t *const fostlib::exceptions::unexpected_eof::message() const
        noexcept {
    return L"Unexpected EOF";
}


#include <fost/exception/unexpected_nil.hpp>
fostlib::exceptions::unexpected_nil::unexpected_nil() noexcept : exception() {}
fostlib::exceptions::unexpected_nil::unexpected_nil(const string &e) noexcept
: exception(e) {}
const wchar_t *const fostlib::exceptions::unexpected_nil::message() const
        noexcept {
    return L"Unexpected zero when processing Unicode stream";
}


#include <fost/exception/unicode_encoding.hpp>
fostlib::exceptions::unicode_encoding::unicode_encoding(const string &e) noexcept
: exception(e) {}
const wchar_t *const fostlib::exceptions::unicode_encoding::message() const
        noexcept {
    return L"Unicode encoding and decoding";
}


#ifdef FOST_OS_WINDOWS
#include "exception-win.cpp"
#else
#include "exception-linux.cpp"
#endif
