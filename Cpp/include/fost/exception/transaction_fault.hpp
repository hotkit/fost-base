/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_TRANSACTION_FAULT_HPP
#define FOST_EXCEPTION_TRANSACTION_FAULT_HPP
#pragma once


#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC transaction_fault : public exception {
        public:
            transaction_fault( const string &error ) throw ();

        protected:
            wliteral const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_TRANSACTION_FAULT_HPP
