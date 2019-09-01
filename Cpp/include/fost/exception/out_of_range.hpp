/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_EXCEPTION_OUT_OF_RANGE_HPP
#define FOST_EXCEPTION_OUT_OF_RANGE_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/detail/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC out_of_range_string : public exception {
          public:
            out_of_range_string(
                    const string &min,
                    const string &max,
                    const string &value) noexcept;
            out_of_range_string(
                    const string &message,
                    const string &min,
                    const string &max,
                    const string &value) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


        template<typename T, typename U = T>
        class out_of_range : public out_of_range_string {
          public:
            out_of_range(const T min, const T max, const U value) noexcept
            : out_of_range_string(
                    coerce<string>(min),
                    coerce<string>(max),
                    coerce<string>(value)) {}
            out_of_range(
                    const string &message,
                    const T min,
                    const T max,
                    const U value) noexcept
            : out_of_range_string(
                    message,
                    coerce<string>(min),
                    coerce<string>(max),
                    coerce<string>(value)) {}
        };


    }


}


#endif // FOST_EXCEPTION_OUT_OF_RANGE_HPP
