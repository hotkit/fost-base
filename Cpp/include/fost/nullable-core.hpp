/*
    Copyright 2001-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_NULLABLE_CORE_HPP
#define FOST_NULLABLE_CORE_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/null.hpp>

#include <experimental/optional>


namespace fostlib {


    namespace detail {


        [[noreturn]] void throw_null_exception();


    }


    template < typename T >
    class nullable {
        std::experimental::optional<T> val;
    public:
        typedef T t_value;
        using value_type = typename std::experimental::optional<T>::value_type;

        /// Construct an empty value
        constexpr nullable()
        : val() {
        }
        /// Construct from the null value as well
        constexpr nullable(t_null)
        : val() {
        }
        /// Construct from a T
        constexpr nullable(T t)
        : val(std::move(t)) {
        }
        /// Converting constructor
        template<typename Y>
        nullable(const nullable<Y> &n)
        : val(n.val) {
        }

        /// Return true if we are holding a value
        constexpr bool has_value() const {
            return static_cast<bool>(val);
        }
        /// Return false if we are holding a value
        [[deprecated("Use not has_value() or not (operator bool)")]]
        bool isnull() const {
            return not has_value();
        }
        /// Allow use in boolean contexts
        explicit constexpr operator bool () const {
            return has_value();
        }

        /// Allow us to assign the null value;
        nullable &operator = (t_null) {
            val = {};
            return *this;
        }
        /// Assign a value
        nullable &operator = (T t) {
            val = std::move(t);
            return *this;
        }
        /// Assign from some other nullable
        template<typename Y>
        nullable &operator = (const nullable<Y> &n) {
            val = n.val;
            return *this;
        }

        /// Check against a null
        bool operator == (t_null) const {
            return not has_value();
        }
        /// Use the super class equality tests
        template<typename Y>
        bool operator == (const Y &rhs) const {
            return val == rhs;
        }
        template<typename Y>
        bool operator == (const nullable<Y> &rhs) const {
            return val == rhs.val;
        }
        /// Not equal
        template<typename Y>
        bool operator != (const Y &rhs) const {
            return val != rhs;
        }

        /// Empty the content, but don't use this
        [[deprecated("Use reset instead")]]
        void set_null() {
            reset();
        }
        void reset() {
            val = {};
        }

        /// Use the parent implementation of value
        const T &value() const {
            try {
                return val.value();
            } catch ( std::experimental::bad_optional_access & ) {
                detail::throw_null_exception();
            }
        }
        /// Return the value or the supplied default
        const T &value_or(const T &v) const {
            return v ? value() : v;
        }
        /// Return a copy when given a default value
        T value_or(T &&v) const {
            if ( val ) v = val.value();
            return v;
        }
        /// Return the value, or the supplied default if there is none
        [[deprecated("Use value_or instead")]]
        const T &value(const T &value) const {
            return val ? val.value() : value;
        }
    };


}


#endif // FOST_NULLABLE_HPP

