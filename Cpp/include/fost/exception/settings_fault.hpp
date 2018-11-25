/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_SETTINGS_FAULT_HPP
#define FOST_EXCEPTION_SETTINGS_FAULT_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC settings_fault : public exception {
          public:
            settings_fault(
                    const string &error,
                    const string &domain,
                    const string &section,
                    const string &name,
                    const string &value) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_SETTINGS_FAULT_HPP
