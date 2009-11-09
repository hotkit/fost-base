/*
    Copyright 2001-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_HPP
#define FOST_EXCEPTION_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/string.hpp>
#include <exception>


namespace fostlib {


    namespace exceptions {


        class FOST_CORE_DECLSPEC FSL_ABSTRACT exception : public std::exception {
        public:
            exception( const exception & ) throw ();
            virtual ~exception() throw ();

            exception &operator =( const exception & );

            virtual ostream &printOn( ostream & ) const;
            const stringstream &info() const;
            stringstream &info();
            const char *what() const throw ();

        protected:
            stringstream m_info;

            exception() throw ();
            explicit exception( const string & ) throw ();

            virtual wliteral const message() const = 0;

        private:
            mutable boost::scoped_array< char > m_what_string;
        };


        class structure_information;
        class FOST_CORE_DECLSPEC structured_handler {
        public:
            structured_handler() throw ();
            ~structured_handler() throw ();

        public:
            boost::scoped_ptr< structure_information > m_info;
        };


       inline ostream &operator<<( ostream &o, const exception &e ) {
            return e.printOn( o );
        }


    }


    FOST_CORE_DECLSPEC void absorbException() throw ();


}


#ifdef WIN32
    #include <fost/exception/win.hpp>
#endif


#endif // FOST_EXCEPTION_HPP

