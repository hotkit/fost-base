/*
    Copyright  2001-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_UNEXPECTED_EOF_HPP
#define FOST_EXCEPTION_UNEXPECTED_EOF_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC unexpected_eof : public exception {
        public:
            unexpected_eof() throw ();
            unexpected_eof(const string &message) throw ();
            unexpected_eof(const string &message, const string &filename) throw ();
            unexpected_eof(const string &message, boost::system::error_code) throw ();

        protected:
            const wchar_t * const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_UNEXPECTED_EOF_HPP

