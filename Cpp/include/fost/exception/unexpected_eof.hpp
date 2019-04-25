/**
    Copyright  2001-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_EXCEPTION_UNEXPECTED_EOF_HPP
#define FOST_EXCEPTION_UNEXPECTED_EOF_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC unexpected_eof : public exception {
          public:
            unexpected_eof() noexcept;
            unexpected_eof(const string &message) noexcept;
            unexpected_eof(
                    const string &message, const string &filename) noexcept;
            unexpected_eof(const string &message, fostlib::error_code) noexcept;

          protected:
            const wchar_t *const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_UNEXPECTED_EOF_HPP
