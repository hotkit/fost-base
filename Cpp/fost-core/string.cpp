/**
    Copyright 2008-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include <mutex> // For c_str() member


namespace {
    /// Historically we've used wchar_t as the UTF16 type on both Linux/Mac
    /// and on Windows, despite it being 32 bit on Linux/Mac. This implements
    /// that until we can deprecate all of this in favour of U16 and U32 APIs.
    std::string stringify(fostlib::wliteral s) {
        std::string r;
        while (s != nullptr && *s) {
            char32_t cp;
            if (f5::cord::is_surrogate(*s)) {
                char16_t const s1 = *s++;
                if (not*s) {
                    throw fostlib::exceptions::not_implemented(
                            __PRETTY_FUNCTION__, "Truncated surrogate pair");
                }
                char16_t const s2 = *s++;
                if (not f5::cord::is_surrogate(s2)) {
                    throw fostlib::exceptions::not_implemented(
                            __PRETTY_FUNCTION__,
                            "Second surrogate isn't a valid surrogate");
                }
                cp = f5::cord::u16decode(s1, s2);
            } else {
                cp = *s++;
            }
            const auto encoded = f5::cord::u8encode(cp);
            r.append(encoded.second.data(), encoded.first);
        }
        return r;
    }
}


/**
 * ## `string` implementation
 */


fostlib::string::string(const string &s, size_type b, size_type c)
: f5::u8string{s.substr(b, c)} {}
fostlib::string::string(wliteral s) : f5::u8string{stringify(s)} {}
fostlib::string::string(wliteral b, wliteral e) {
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}
fostlib::string::string(size_type l, char32_t c)
: f5::u8string{[](size_type l, char32_t c) {
      const auto encoded = f5::cord::u8encode(c);
      std::string r;
      r.reserve(encoded.first * l);
      while (l--) { r.append(encoded.second.data(), encoded.first); }
      return f5::u8string{std::move(r)};
  }(l, c)} {}


char const *fostlib::string::c_str() const {
    static std::mutex cmutex;
    std::lock_guard lock{cmutex};
    if (cstring == nullptr || *this != ccstring) {
        ccstring = f5::u8string{*this};
        cstring = ccstring.shrink_to_fit();
    }
    return cstring;
}


fostlib::string fostlib::string::operator+(char32_t c) const {
    auto const encoded = f5::cord::u8encode(c);
    std::string r;
    r.reserve(bytes() + encoded.first);
    r.append(memory().begin(), memory().end());
    r.append(encoded.second.data(), encoded.first);
    return f5::u8string{std::move(r)};
}
fostlib::string fostlib::string::operator+(wliteral s) const {
    std::string r;
    r.reserve(bytes() + std::wcslen(s)); // Approximation
    r.append(memory().begin(), memory().end());
    r.append(stringify(s));
    return f5::u8string{std::move(r)};
}


bool fostlib::string::operator==(wliteral rl) const {
    f5::u8string const r{stringify(rl)};
    return *this == f5::u8view{r};
}
bool fostlib::string::operator<(wliteral rl) const {
    f5::u8string const r{stringify(rl)};
    return *this < f5::u8view{r};
}
bool fostlib::string::operator<=(wliteral rl) const {
    f5::u8string const r{stringify(rl)};
    return *this <= f5::u8view{r};
}
bool fostlib::string::operator>(wliteral rl) const {
    f5::u8string const r{stringify(rl)};
    return *this > f5::u8view{r};
}
bool fostlib::string::operator>=(wliteral rl) const {
    f5::u8string const r{stringify(rl)};
    return *this >= f5::u8view{r};
}


fostlib::string &fostlib::string::erase(size_type c) {
    return *this = substr(c);
}
fostlib::string &fostlib::string::erase(size_type b, size_type c) {
    return *this = substr(0, b) + substr(b + c);
}
fostlib::string &fostlib::string::insert(size_type b, const string &t) {
    return *this = substr(0, b) + t + substr(b);
}
fostlib::string &fostlib::string::replace(
        size_type b, size_type e, const string &s, size_type sb, size_type se) {
    return *this = substr(0, b) + s.substr(sb, sb + se) + substr(b + e);
}

char32_t fostlib::string::at(std::size_t p) const {
    size_type index{};
    for (char32_t c : *this) {
        if (index++ == p) { return c; }
    }
    throw exceptions::not_implemented(__PRETTY_FUNCTION__);
}


fostlib::string::size_type fostlib::string::find(char32_t const f) const {
    size_type index{};
    for (char32_t c : *this) {
        if (c == f) { return index; }
        ++index;
    }
    return npos;
}
fostlib::string::size_type
        fostlib::string::find(const string &f, size_type off) const {
    size_type index{};
    for (auto pos = begin(), e = end(); pos != e; ++pos, ++index) {
        if (index >= off && f5::u8string{pos, e}.starts_with(f)) return index;
    }
    return npos;
}
fostlib::string::size_type
        fostlib::string::find_first_of(const string &t) const {
    size_type index{};
    for (char32_t c : *this) {
        if (t.find(c) != npos) { return index; }
        ++index;
    }
    return npos;
}
fostlib::string::size_type
        fostlib::string::find_first_not_of(const string &t) const {
    size_type index{};
    for (char32_t c : *this) {
        if (t.find(c) == npos) { return index; }
        ++index;
    }
    return npos;
}
fostlib::string::size_type
        fostlib::string::find_last_not_of(const string &t) const {
    size_type index{}, found{npos};
    for (char32_t c : *this) {
        if (t.find(c) == npos) { found = index; }
        ++index;
    }
    return found;
}


/**
 * ## Non-members
 */


fostlib::string fostlib::operator+(wliteral s, const string &r) {
    return fostlib::string{s} + r;
}
