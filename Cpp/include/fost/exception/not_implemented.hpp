/*
    Copyright  2001-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_EXCEPTION_NOT_IMPLEMENTED_HPP
#define FOST_EXCEPTION_NOT_IMPLEMENTED_HPP
#pragma once


#include <fost/exception.hpp>

#include <boost/system/error_code.hpp>


namespace fostlib {


    namespace exceptions {


        /// Not implemented exceptions show where code is still incomplete
        class FOST_CORE_DECLSPEC not_implemented : public exception {
        public:
            /// Throw a not implemented exception
            not_implemented() throw ();
            /// Throw a not implemented exception providing a function name
            not_implemented( const string &function ) throw ();
            /// Throw a not implemented exception providing a function name and a message
            not_implemented( const string &function, const string &message ) throw ();
            /// Throw providing a function name, message and extra information
            not_implemented(
                const string &function, const string &message, const string &extra
            ) throw ();
            /// Allow us to throw from a Boost error code
            not_implemented(
                const string &function, boost::system::error_code error
            ) throw ();
            /// Allow us to throw from a Boost error code with a message
            not_implemented(
                const string &function, boost::system::error_code error,
                const string &message
            ) throw ();

        protected:
            const wchar_t * const message() const throw ();
        };


    }


}


#endif // FOST_EXCEPTION_NOT_IMPLEMENTED_HPP

