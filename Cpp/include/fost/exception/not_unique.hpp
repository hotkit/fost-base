/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_EXCEPTION_NOT_UNIQUE_HPP
#define FOST_EXCEPTION_NOT_UNIQUE_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC not_unique : public exception {
          public:
            not_unique(const string &error) noexcept;
            not_unique(
                    const string &error,
                    const string &context,
                    const string &alternative1,
                    const string &alternative2) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NOT_UNIQUE_HPP
