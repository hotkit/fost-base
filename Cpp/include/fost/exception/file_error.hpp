/**
    Copyright  2001-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_EXCEPTION_FILE_ERROR_HPP
#define FOST_EXCEPTION_FILE_ERROR_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/detail/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC file_error : public exception {
          public:
            /// Throw an error about the given filename
            file_error(const string &message, const string &filename) noexcept;
            /// Thow an error from an error code
            file_error(
                    const string &message,
                    const fostlib::fs::path &,
                    fostlib::error_code) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_FILE_ERROR_HPP
