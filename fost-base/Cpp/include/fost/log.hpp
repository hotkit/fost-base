/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_LOG_HPP
#define FOST_LOG_HPP
#pragma once


#include <fost/json.hpp>
#include <fost/timestamp.hpp>


namespace fostlib {


    /// Stores the main parts of the logging library
    namespace logging {


        /// The logging messages themselves
        struct FOST_CORE_DECLSPEC message {
            /// Create a message from this data
            message(std::size_t level, nliteral name, const json &data);
            /// Create a message from this data
            message(const string &module,
                std::size_t level, nliteral name, const json &data);

            /// When the message was recorded
            accessors< const timestamp > when;
            /// The module name that the message is for
            accessors< nullable< string > > module;
            /// The level of the logging message
            accessors< std::size_t > level;
            /// The name of the logging message
            accessors< nliteral > name;
            /// The body data of the logging message
            accessors< json > body;
        };


        /// Log a logging message
        FOST_CORE_DECLSPEC
        void log(const message &);

        /// Log to a certain level
        template< typename L, typename J >
        void log( L level, const J &value ) {
            level(value);
        }

        /// Add a message to the logs at a given level
        FOST_CORE_DECLSPEC inline
        void log(std::size_t level, nliteral name, const fostlib::json &data) {
            logging::log(message(level, name, data));
        }


        /// A sink is used to capture logging data
        template< typename F >
        class sink {
        };


        /// Used to create a logging level
        #define FSL_DEFINE_LOGGING_LEVEL( N, value ) \
            const struct N##_level_tag { \
                static const std::size_t level() { return value; } \
                static fostlib::nliteral name() { return #N; } \
                template< typename J > void operator () (const J &v) const { \
                    fostlib::logging::log(level(), name(), fostlib::json(v)); \
                } \
            } N = {};

        /// The debug level logger
        FSL_DEFINE_LOGGING_LEVEL(debug, 0x100u);
        /// The info level logger
        FSL_DEFINE_LOGGING_LEVEL(info, 0x400u);
        /// The warning level logger
        FSL_DEFINE_LOGGING_LEVEL(warning, 0x1000u);
        /// The error level logger
        FSL_DEFINE_LOGGING_LEVEL(error, 0x4000u);
        /// The critical level logger
        FSL_DEFINE_LOGGING_LEVEL(critical, 0x10000u);


    }


}


#endif // FOST_LOG_HPP
