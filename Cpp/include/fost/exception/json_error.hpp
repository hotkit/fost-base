/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_EXCEPTION_JSON_ERROR_HPP
#define FOST_EXCEPTION_JSON_ERROR_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC json_error : public exception {
          public:
            json_error(const string &message) noexcept;
            json_error(const string &message, const json &value) noexcept;

          protected:
            wliteral const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_JSON_ERROR_HPP
