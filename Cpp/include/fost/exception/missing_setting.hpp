/**
    Copyright 2001-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_EXCEPTION_MISSING_SETTING_HPP
#define FOST_EXCEPTION_MISSING_SETTING_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC missing_setting : public exception {
          public:
            missing_setting(const string &section, const string &name) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_MISSING_SETTING_HPP
