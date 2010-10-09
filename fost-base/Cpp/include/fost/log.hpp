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


namespace fostlib {


    /// Stores the main parts of the logging library
    namespace logging {


        /// Add a message to the logs at a given level
        void log(std::size_t level, nliteral name, const fostlib::json &message);

        /// Used to create a logging level
        #define FSL_DEFINE_LOGGING_LEVEL( N, value ) \
            const struct N##_level_tag { \
                static const std::size_t level = value; \
                template< typename J > void operator () (const J &v) const { \
                    fostlib::logging::log(level, #N, fostlib::json(v)); \
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


        /// Log to a certain level
        template< typename L, typename J >
        void log( L level, const J &value ) {
            level(value);
        }


    }


}


#endif // FOST_LOG_HPP
