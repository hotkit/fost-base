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


    template < typename T >
    class nullable {
        std::experimental::optional<T> val;
    public:
        typedef T t_value;
        using value_type = typename std::experimental::optional<T>::value_type;

        /// Construct an empty value
        nullable()
        : val() {
        }
        /// Construct from the null value as well
        nullable(t_null)
        : val() {
        }

        /// Perfect forward other arguments to parent
        template<typename ...Args>
        nullable(Args&&... args)
        : val(std::forward<Args&&...>(args...)) {
        }

        /// Return false if we are holding a value
        bool isnull() const {
            return not static_cast<bool>(val);
        }

        /// We can just use the super class assignments
        template<typename Y>
        auto operator = (Y &&y) {
            return val = std::forward<Y&&>(y);
        }
        /// Allow us to assign the null value;
        nullable &operator = (t_null) {
            set_null();
            return *this;
        }
        /// Use the super class equality tests
        template<typename Y>
        bool operator == (const Y &rhs) const {
            return val == rhs;
        }
        /// Not equal
        template<typename Y>
        bool operator != (const Y &rhs) const {
            return val != rhs;
        }
        /// Compare two nullables
        bool operator == (const nullable &n) const {
            return val == n.val;
        }
        /// Not equal for two nullables
        bool operator != (const nullable &n) const {
            return val != n.val;
        }

        /// Empty the content
        void set_null() {
            val = {};
        }

        /// Use the parent implementation of value
        auto value() const {
            return val.value();
        }
        auto value( const T &value ) const {
            return val.value_or(value);
        }
    };


}


#endif // FOST_NULLABLE_HPP

