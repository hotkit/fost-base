/**
    Copyright 2008-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"


namespace {
    std::string stringify(fostlib::wliteral s) {
        std::string r;
        while ( s != nullptr && *s ) {
            const auto encoded = f5::cord::u8encode(*s++);
            r.append(encoded.second.data(), encoded.first);
        }
        return r;
    }
}


/**
 * ## `string` implementation
 */


fostlib::string::string(wliteral s)
: f5::u8string{stringify(s)} {}
fostlib::string::string(size_type l, char32_t c) {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}


char const *fostlib::string::c_str() const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}


fostlib::string fostlib::string::operator+(wliteral s) const {
    std::string r;
    r.reserve(bytes() + std::wcslen(s));
    r.append(memory().begin(), memory().end());
    while ( s != nullptr && *s ) {
        const auto encoded = f5::cord::u8encode(*s++);
        r.append(encoded.second.data(), encoded.first);
    }
    return f5::u8string{std::move(r)};
}


bool fostlib::string::operator == (wliteral r) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}


char32_t fostlib::string::at(std::size_t) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}


fostlib::string::size_type fostlib::string::find(const string &f, size_type off) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}
fostlib::string::size_type fostlib::string::find_first_of(const string &) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}
fostlib::string::size_type fostlib::string::find_first_not_of(const string &) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}
fostlib::string::size_type fostlib::string::find_last_not_of(const string &) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}
bool fostlib::string::endswith(const string &) const {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}


/**
 * ## Non-members
 */


fostlib::string fostlib::operator+(wliteral s, const string &r) {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}
