/*
    Copyright 2001-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_NULL_HPP
#define FOST_EXCEPTION_NULL_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC null : public exception {
        public:
            null() noexcept;
            null( const string &message ) noexcept;
            null( const string &message, const string &info ) noexcept;

        protected:
            const wchar_t * const message() const noexcept;
        };


    }


}


#endif // FOST_EXCEPTION_NULL_HPP

