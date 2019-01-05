/**
    Copyright 2001-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_STRING_HPP
#define FOST_STRING_HPP
#pragma once


#include <fost/string-fwd.hpp>
#include <f5/cord/iostream.hpp>


namespace fostlib {


    /// ## String
    /**
        Interim wrapper around u8string. The intention is that we merge the
        types together so that we can switch over to u8string later on.
    */
    class string : private f5::u8string {
        /// These two members are only used for the implementation
        /// of `c_str()`. We must cache the C string version so that we
        /// have an idempotent implementation of the member.
        mutable u8string ccstring = {};
        mutable char const *cstring = nullptr;

      public:
        using size_type = std::size_t;
        /// Marker for end of string
        constexpr static const size_type npos =
                std::numeric_limits<size_type>::max();

        /// ### Constructors
        using u8string::u8string;
        string(u8string s) : u8string{s} {}
        string(std::string s) : u8string{std::move(s)} {}
        string(nliteral s) : u8string{std::string(s)} {}
        string(wliteral);
        string(wchar_t c) : string{1u, char32_t(c)} {}

        string(size_type, char32_t);

        string(nliteral b, nliteral e) : u8string{std::string{b, e}} {}
        string(wliteral, wliteral);
        string(const string &, size_type, size_type = npos);
        string(std::string::const_iterator b, std::string::const_iterator e)
        : u8string{std::string{b, e}} {}

        /// ### Conversions
        operator f5::u8view() const { return f5::u8view{u8string{*this}}; }
        operator std::string() const {
            return u8string::operator std::string();
        }
        std::string std_str() const { return static_cast<std::string>(*this); }
        /**
            `c_str()` is particularly problematic to bridge because we don't
            have storage for it. For the `const` version we just have to have
            storage for it, and we need to deal with it properly :-( Clearly
           we're going to want to deprecate the `const` version ASAP
         */
        char const *c_str() const;

        /// ### Simple query APIs
        using u8string::bytes;
        using u8string::empty;
        using u8string::memory;
        auto data() const { return memory(); }
        size_type length() const { return code_points(); }
        size_type native_length() const { return bytes(); }

        /// ### String operations
        string operator+(f5::u8view v) const {
            return f5::u8view{*this} + v;
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
        using u8string::begin;
        using u8string::const_iterator;
        using u8string::end;

        /// ### Comparison operators
        using u8string::operator==;
        bool operator==(const string &r) const { return *this == f5::u8view{r}; }
        bool operator==(nliteral r) const {
            return f5::u8view{*this} == f5::u8view{r, std::strlen(r)};
        }
        bool operator==(wliteral) const;
        using u8string::operator!=;
        bool operator!=(nliteral r) const { return not(*this == r); }
        bool operator!=(wliteral r) const  { return not(*this == r); }
        using u8string::operator<;
        bool operator<(nliteral r) const  {
            return f5::u8view{*this} < f5::u8view{r, std::strlen(r)};
        }
        bool operator<(wliteral) const;
        using u8string::operator<=;
        bool operator<=(nliteral r) const {
            return f5::u8view{*this} <= f5::u8view{r, std::strlen(r)};
        }
        bool operator<=(wliteral) const;
        using u8string::operator>;
        bool operator>(nliteral r) const {
            return f5::u8view{*this} > f5::u8view{r, std::strlen(r)};
        }
        bool operator>(wliteral) const;
        using u8string::operator>=;
        bool operator>=(nliteral r) const {
            return f5::u8view{*this} >= f5::u8view{r, std::strlen(r)};
        }
        bool operator>=(wliteral) const;

        /// ### Algorithmic APIs
        size_type find(char32_t) const;
        size_type find(const string &, size_type = 0u) const;
        size_type find_first_of(const string &) const;
        size_type find_first_not_of(const string &) const;
        size_type find_last_not_of(const string &) const;
        using u8string::starts_with;
        bool startswith(const string &s) const {
            return starts_with(f5::u8view{s});
        }
        using u8string::ends_with;
        bool endswith(const string &s) const {
            return ends_with(f5::u8view{s});
        }

        /// ### Mutation APIs (to be deprecated)
        string &operator+=(wchar_t c) {
            return *this = (*this + c);
        }
        string &operator+=(const string &s) {
            return *this = f5::u8view{*this} + f5::u8view{s};
        }
        string &clear() { return *this = u8string{}; }
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
        string substr() const {
            return *this;
        }
        string substr(size_type b) const {
            return u8string::substr(b);
        }
        string substr(size_type b, size_type c) const {
            return u8string::substr(b, b + c);
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
    inline bool operator==(f5::lstring l, const string &r) { return r == f5::u8view{l}; }
    inline bool operator==(nliteral l, const string &r) { return r == l; }
    inline bool operator==(wliteral l, const string &r) { return r == l; }
    inline bool operator!=(wliteral l, const string &r) { return r != l; }
    inline bool operator<(nliteral l, const string &r) {
        return f5::u8view{l, std::strlen(l)} < f5::u8view{r};
    }
    inline bool operator<(wliteral l, const string &r) { return string{l} < r; }
    inline bool operator<=(nliteral l, const string &r) { return not(r < l); }
    inline bool operator<=(wliteral l, const string &r) { return not(r < l); }
    inline bool operator>(nliteral l, const string &r) { return r <= l; }
    inline bool operator>(wliteral l, const string &r) { return r <= l; }
    inline bool operator>=(nliteral l, const string &r) { return not(l < r); }
    inline bool operator>=(wliteral l, const string &r) { return not(l < r); }
    inline string operator+(nliteral l, const string &r) {
        return f5::u8view{l, std::strlen(l)} + f5::u8view{r};
    }
    string operator+(wliteral l, const string &r);

    bool operator==(f5::u8string, wliteral r);


}


#ifdef FOST_COERCE_HPP
#include <fost/string/coerce.hpp>
#endif
#ifdef FOST_ACCESSORS_HPP
#include <fost/string/accessors.hpp>
#endif
#ifdef FOST_NULLABLE_HPP
#include <fost/string/nullable.hpp>
#endif



namespace std {


    /// Allow the string to be output to a stream
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const fostlib::string &s) {
        return o << s.std_str();
    }
    /// Allow the non-native string literal to be output to a stream
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, fostlib::non_native_literal lit) {
        return o << fostlib::string(lit);
    }


}

#endif // FOST_STRING_HPP
