/*
    Copyright  2001-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
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
            file_error(const string &message, const boost::filesystem::path &,
                boost::system::error_code) noexcept;

        protected:
            const wchar_t * const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_FILE_ERROR_HPP

