/*
    Copyright  2001-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_UTILITY_NULLABLE_HPP
#define FOST_STRING_UTILITY_NULLABLE_HPP
#pragma once


#include <fost/string.hpp>
#include <fost/nullable.hpp>


namespace fostlib {


    /// Removes white space from the start and end of a string
    FOST_CORE_DECLSPEC nullable<utf8_string> trim(const utf8_string &text);
    /// Removes white space from the start and end of a string
    FOST_CORE_DECLSPEC nullable<string> trim(const string &text);
    /// Removes white space from the start and end of a string
    inline nullable<string> trim(wliteral text) { return trim(string(text)); }
    /// Removes white space from the start and end of a string
    FOST_CORE_DECLSPEC nullable<string>
            trim(const string &text, const string &chars);
    /// Removes white space from the start and end of a string
    FOST_CORE_DECLSPEC nullable<utf8_string>
            trim(const nullable<utf8_string> &text);
    /// Removes white space from the start and end of a string
    FOST_CORE_DECLSPEC nullable<string> trim(const nullable<string> &text);


    /// Concatenate two strings with a separator (if needed)
    FOST_CORE_DECLSPEC nullable<string>
            concat(const nullable<f5::u8view> &left,
                   f5::u8view mid,
                   const nullable<f5::u8view> &right);
    /// Concatenate two strings
    FOST_CORE_DECLSPEC nullable<string>
            concat(const nullable<f5::u8view> &left,
                   const nullable<f5::u8view> &right);

    /// Replace all occurrences of the requested string with the specified one
    FOST_CORE_DECLSPEC utf8_string replace_all(
            const utf8_string &text,
            f5::u8view findThis,
            const nullable<f5::u8view> &replaceWith = fostlib::null);
    /// Replace all occurrences of the requested string with the specified one
    FOST_CORE_DECLSPEC string replace_all(
            const string &text,
            f5::u8view findThis,
            const nullable<f5::u8view> &replaceWith = fostlib::null);
    /// Replace all occurrences of the requested string with the specified one
    FOST_CORE_DECLSPEC nullable<string> replace_all(
            const nullable<f5::u8view> &text,
            f5::u8view findThis,
            const nullable<f5::u8view> &replaceWith = fostlib::null);


    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const utf8_string &text, f5::u8view bound);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const nullable<utf8_string> &text, f5::u8view bound);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const utf8_string &text);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<utf8_string, nullable<utf8_string>>
            partition(const nullable<utf8_string> &text);

    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>>
            partition(const string &text, f5::u8view bound);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>>
            partition(const nullable<string> &text, f5::u8view bound);
    /// Partition (and trim) a string into two parts around the first space
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>> partition(const string &text);
    /// Partition (and trim) a string into two parts around the boundary
    FOST_CORE_DECLSPEC
    std::pair<string, nullable<string>> partition(const nullable<string> &text);

    /// Split a string in the form `first OPEN second CLOSE` returning first,
    /// second
    FOST_CORE_DECLSPEC std::pair<string, nullable<string>>
            crack(f5::u8view text, f5::u8view open, f5::u8view close);


}


#endif // FOST_STRING_UTILITY_NULLABLE_HPP
