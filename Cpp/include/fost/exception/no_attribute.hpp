/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_NO_ATTRIBUTE_HPP
#define FOST_EXCEPTION_NO_ATTRIBUTE_HPP
#pragma once


#include <fost/core>


namespace fostlib {


    namespace exceptions {


        class FOST_SCHEMA_DECLSPEC no_attribute : public exception {
        public:
            no_attribute( const string &attribute ) throw ();

        protected:
            wliteral const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_NO_ATTRIBUTE_HPP

