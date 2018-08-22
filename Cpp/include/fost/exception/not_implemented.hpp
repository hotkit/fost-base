/*
    Copyright 2001-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <fost/coerce.hpp>
#include <fost/exception.hpp>


namespace fostlib {


    namespace exceptions {


        /// Not implemented exceptions show where code is still incomplete
        class FOST_CORE_DECLSPEC not_implemented : public exception {
        public:
            /// Throw a not implemented exception
            not_implemented() noexcept;
            /// Throw a not implemented exception providing a function name
            not_implemented( const string &function ) noexcept;
            /// Throw a not implemented exception providing a function name and a message
            not_implemented( const string &function, const string &message ) noexcept;
            /// Throw providing a function name, message and extra information
            not_implemented(
                const string &function, const string &message, const string &extra
            ) noexcept;
            /// Throw providing a functio name, message and extra JSON information
            not_implemented(
                const string &function, const string &message, const json &extra);
            /// Extra information can be anything coercible to JSON
            template<typename E>
            not_implemented(
                const string &function, const string &message, const E &extra)
            : not_implemented(function, message, coerce<json>(extra)) {
            }
            /// Allow us to throw from a Boost error code
            not_implemented(
                const string &function, boost::system::error_code error
            ) noexcept;
            /// Allow us to throw from a Boost error code with a message
            not_implemented(
                const string &function, boost::system::error_code error,
                const string &message
            ) noexcept;

        protected:
            const wchar_t * const message() const noexcept;
        };


    }


}

