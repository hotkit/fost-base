/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_NOT_NULL_HPP
#define FOST_EXCEPTION_NOT_NULL_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC not_null : public exception {
        public:
            not_null() noexcept;
            not_null( const string &message ) noexcept;
            not_null( const string &message, const string &info ) noexcept;

        protected:
            wliteral const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NOT_NULL_HPP

