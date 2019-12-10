/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_EXCEPTION_PARSE_ERROR_HPP
#define FOST_EXCEPTION_PARSE_ERROR_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC parse_error : public exception {
          public:
            parse_error(string const &message) noexcept;
            parse_error(string const &, const string &value) noexcept;
            parse_error(string const &, std::size_t line, std::size_t col);
            parse_error(
                    string const &,
                    fostlib::fs::path const &filename,
                    std::size_t line,
                    std::size_t col);

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_PARSE_ERROR_HPP
