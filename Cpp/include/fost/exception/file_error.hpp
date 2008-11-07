/*
    Copyright  2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_FILE_ERROR_HPP
#define FOST_EXCEPTION_FILE_ERROR_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/string/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC file_error : public exception {
        public:
            file_error( const string &message, const string &filename ) throw ();

        protected:
            const wchar_t * const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_FILE_ERROR_HPP

