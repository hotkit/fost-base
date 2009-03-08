/*
    Copyright  2001-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_OVERFLOW_HPP
#define FOST_EXCEPTION_OVERFLOW_HPP
#pragma once


#include <fost/exception.hpp>
#include <fost/string/coerce.hpp>


namespace fostlib {


    namespace exceptions {


        template< typename T >
        class overflow;

        template<>
        class FOST_CORE_DECLSPEC overflow< string > : public exception {
        public:
            overflow( const string &message ) throw ();
            overflow( const string &message, const string &n, const string &d, const string &m ) throw ();
            overflow( const string &n, const string &d, const string &m ) throw ();

        protected:
            const wchar_t * const message() const throw ();
        };

        template< typename T >
        class overflow : public overflow< string > {
        public:
            overflow( const string &message, const T n, const T d, const T m ) throw ()
            : overflow< string >( m, coerce< string >( n ), coerce< string >( d ), coerce< string >( m ) ) {
            }
            overflow( const T n, const T d, const T m ) throw ()
            : overflow< string >( coerce< string >( n ), coerce< string >( d ), coerce< string >( m ) ) {
            }
        };


    }


}


#endif // FOST_EXCEPTION_OVERFLOW_HPP

