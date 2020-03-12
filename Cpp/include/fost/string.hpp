/**
    Copyright 2001-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_STRING_HPP
#define FOST_STRING_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <f5/cord/iostream.hpp>

#include <ostream>


namespace fostlib {


    /// ## String
    /**
        Interim wrapper around u8string. The intention is that we merge the
        types together so that we can switch over to u8string later on.
    */
    class string : public f5::u8string {
      public:
        using size_type = std::size_t;
        /// Marker for end of string
        constexpr static const size_type npos =
                std::numeric_limits<size_type>::max();

        /// ### Constructors
        using f5::u8string::u8string;
        string(f5::u8string s) : f5::u8string{s} {}
        string(std::string s) : f5::u8string{std::move(s)} {}
        explicit string(std::string_view s) : string{std::string{s}} {}
        string(nliteral s) : f5::u8string{std::string(s)} {}
        string(wliteral);
        explicit string(char32_t c) : string{1u, char32_t(c)} {}

        string(size_type, char32_t);

        [[deprecated("Switch to something like f5::u8view")]] string(
                nliteral b, nliteral e)
        : f5::u8string{std::string{b, e}} {}
        string(const string &, size_type, size_type = npos);
        string(std::string::const_iterator b, std::string::const_iterator e)
        : f5::u8string{std::string{b, e}} {}

        /// ### Conversions
        using f5::u8string::operator std::string_view;
        using f5::u8string::operator std::string;
        using f5::u8string::operator f5::u8view;
        using f5::u8string::shrink_to_fit;

        /// We also need to allow this type to be changed to its super class
        /// to help the transition over to this implementation. Because we
        /// want the inheritance to be private (to stop all sorts of problematic
        /// things from happening), and we want to be able to use `u8string`
        /// in new code, we need a way to cleanly convert over. This is done
        /// in a new function that can be deprecated once the rest of the
        /// old string API has been removed.
        f5::u8string u8string_transition() const { return f5::u8string{*this}; }

        /// ### Simple query APIs
        using f5::u8string::bytes;
        using f5::u8string::empty;
        using f5::u8string::memory;
        auto data() const { return memory(); }
        using f5::u8string::code_points;
        [
                [deprecated("Use memory().size() or code_points() as "
                            "appropriate")]] size_type
                length() const {
            return code_points();
        }
        [[deprecated("Use code_points()")]] size_type native_length() const {
            return bytes();
        }

        /// ### String operations
        string operator+(f5::u8view v) const { return f5::u8view{*this} + v; }
        string operator+(f5::lstring v) const {
            return f5::u8view{*this} + f5::u8view{v};
        }
        string operator+(string const &v) const {
            return f5::u8view{*this} + f5::u8view{v};
        }
        string operator+(char32_t) const;
        string operator+(nliteral r) const {
            return f5::u8view{*this} + f5::u8view{r, std::strlen(r)};
        }
        string operator+(wliteral) const;
        /// Because our underlying string type is immutable it makes no sense
        /// to do any reservation.
        void reserve(size_type) {}

        /// ### Iteration based APIs
        using f5::u8string::begin;
        using f5::u8string::const_iterator;
        using f5::u8string::end;

        /// ### Comparison operators
        friend bool operator==(string const &l, string const &r) {
            return f5::u8view{l} == f5::u8view{r};
        }
        /// We need these explicitly here to resolve ambiguities otherwise
        /// things get too complicated The implementation in f5::u8string is
        /// generic though, so that's good
        friend bool operator==(string const &l, f5::u8view r) {
            return f5::u8view{l} == r;
        }
        friend bool operator==(string const &l, std::string const &r) {
            return f5::u8view{l} == r;
        }
        friend bool operator==(string const &l, char const *r) {
            return f5::u8view{l} == r;
        }
        friend bool operator==(f5::u8string l, string const &r) {
            return l == f5::u8view{r};
        }
        friend bool operator==(f5::lstring l, string const &r) {
            return l == f5::u8view{r};
        }
        template<size_type N>
        friend bool operator==(string const &l, char const (&r)[N]) {
            return f5::u8view{l} == r;
        }
        template<size_type N>
        friend bool operator==(char const (&l)[N], string const &r) {
            return l == f5::u8view{r};
        }
        bool operator==(wliteral) const;
        template<size_type N>
        bool operator==(wchar_t const (&l)[N]) const {
            return *this == reinterpret_cast<wliteral>(l);
        }

        friend bool operator!=(string const &l, string const &r) {
            return f5::u8view{l} != f5::u8view{r};
        }
        friend bool operator!=(f5::u8view l, string const &r) {
            return l != f5::u8view{r};
        }
        template<size_type N>
        friend bool operator!=(string const &l, char const (&r)[N]) {
            return f5::u8view{l} != r;
        }
        template<size_type N>
        friend bool operator!=(char const (&l)[N], string const &r) {
            return l != f5::u8view{r};
        }
        bool operator!=(wliteral r) const { return not(*this == r); }

        using f5::u8string::operator<;
        bool operator<(nliteral r) const {
            return f5::u8view{*this} < f5::u8view{r, std::strlen(r)};
        }
        bool operator<(wliteral) const;

        using f5::u8string::operator<=;
        bool operator<=(nliteral r) const {
            return f5::u8view{*this} <= f5::u8view{r, std::strlen(r)};
        }
        bool operator<=(wliteral) const;

        using f5::u8string::operator>;
        bool operator>(nliteral r) const {
            return f5::u8view{*this} > f5::u8view{r, std::strlen(r)};
        }
        bool operator>(wliteral) const;

        using f5::u8string::operator>=;
        bool operator>=(nliteral r) const {
            return f5::u8view{*this} >= f5::u8view{r, std::strlen(r)};
        }
        bool operator>=(wliteral) const;

        /// ### Algorithmic APIs
        size_type find(char32_t, size_type = 0u) const;
        size_type find(const string &, size_type = 0u) const;
        size_type find_first_of(char32_t c) const { return find(c); }
        size_type find_first_of(const string &) const;
        size_type find_first_not_of(const string &) const;
        size_type find_last_not_of(const string &) const;
        using f5::u8string::starts_with;
        bool startswith(const string &s) const {
            return starts_with(f5::u8view{s});
        }
        bool startswith(char32_t c) const {
            return not empty() && *begin() == c;
        }
        using f5::u8string::ends_with;
        bool endswith(const string &s) const {
            return ends_with(f5::u8view{s});
        }
        bool endswith(char32_t c) const { return ends_with(string{c}); }

        /// ### Mutation APIs (to be deprecated)
        string &operator+=(wchar_t c) { return *this = (*this + c); }
        string &operator+=(const string &s) {
            return *this = f5::u8view{*this} + f5::u8view{s};
        }
        string &clear() { return *this = f5::u8string{}; }
        string &erase() { return clear(); }
        string &erase(size_type s); // Erase from s to end
        string &erase(size_type s, size_type c); // Erase c chars from s
        string &insert(size_type, const string &);
        string &
                replace(size_type,
                        size_type,
                        const string &,
                        size_type = 0u,
                        size_type = npos);

        /// ### Substrings and slicing
        string substr() const { return *this; }
        string substr(size_type b) const { return f5::u8string::substr(b); }
        string substr(size_type b, size_type c) const {
            return f5::u8string::substr_pos(b, b + c);
        }
        char32_t at(size_type) const;
        char32_t operator[](size_type p) const { return at(p); }
    };


    /// ADL `std::size`and `std::data`  implementations
    inline auto size(const string &s) { return s.bytes(); }
    inline auto data(const string &s) {
        return reinterpret_cast<char const *>(s.memory().data());
    }


    /// ### Binary operators needed outside of the class
    inline f5::u8string operator+(f5::u8view l, string const &r) {
        return l + f5::u8view{r};
    }
    inline f5::u8string operator+(f5::u8string l, string const &r) {
        return f5::u8view{l} + f5::u8view{r};
    }
    inline bool operator==(wliteral l, const string &r) { return r == l; }
    inline bool operator!=(wliteral l, const string &r) { return r != l; }
    inline bool operator<(nliteral l, const string &r) {
        return f5::u8view{l, std::strlen(l)} < r;
    }
    inline bool operator<(wliteral l, const string &r) { return string{l} < r; }
    inline bool operator<=(nliteral l, const string &r) { return not(r < l); }
    inline bool operator<=(wliteral l, const string &r) { return not(r < l); }
    inline bool operator>(nliteral l, const string &r) { return r <= l; }
    inline bool operator>(wliteral l, const string &r) { return r <= l; }
    inline bool operator>=(nliteral l, const string &r) { return not(l < r); }
    inline bool operator>=(wliteral l, const string &r) { return not(l < r); }
    inline string operator+(nliteral l, const string &r) {
        return f5::u8view{l, std::strlen(l)} + r;
    }
    string operator+(wliteral l, const string &r);

    bool operator==(f5::u8string, wliteral r);


}


namespace std {


    /// Allow the non-native string literal to be output to a stream
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, fostlib::non_native_literal lit) {
        return o << fostlib::string(lit);
    }


}

#endif // FOST_STRING_HPP
