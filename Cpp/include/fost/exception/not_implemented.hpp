/*
    Copyright  2001-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_NOT_IMPLEMENTED_HPP
#define FOST_EXCEPTION_NOT_IMPLEMENTED_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC not_implemented : public exception {
        public:
            not_implemented() throw ();
            not_implemented( const string &function ) throw ();
            not_implemented( const string &function, const string &message ) throw ();
            not_implemented( const string &function, const string &message, const string &extra ) throw ();

        protected:
            const wchar_t * const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_NOT_IMPLEMENTED_HPP

