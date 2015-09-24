/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_LOG_MESSAGE_HPP
#define FOST_LOG_MESSAGE_HPP
#pragma once


#include <fost/module.hpp>
#include <fost/timestamp.hpp>


namespace fostlib {


    namespace log {


        /// The logging messages themselves
        struct FOST_CORE_DECLSPEC message {
            /// Create a message from this data
            [[deprecated("Pass a fostlib::module as the first argument")]]
            message(std::size_t, nliteral, const json &);
            /// Create a message from this data
            message(const fostlib::module &, std::size_t, nliteral, const json &);

            /// When the message was recorded
            accessors< const timestamp > when;
            /// The module name that the message is for
            const fostlib::module &module() const {
                return m_module;
            }
            /// The level of the logging message
            accessors< std::size_t > level;
            /// The name of the logging message
            accessors< nliteral > name;
            /// The body data of the logging message
            accessors< json > body;

        private:
            const fostlib::module &m_module;
        };


    }


    /// Allow us to turn a logging::message into a JSON blob
    template<>
    struct FOST_CORE_DECLSPEC coercer<json, fostlib::log::message> {
        json coerce(const fostlib::log::message &);
    };


}


#endif // FOST_LOG_MESSAGE_HPP
