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
        void log(std::size_t level, const fostlib::json &message);


        /// The debug level logger
        const struct debug_tag {
            static const std::size_t level = 1000u;

            template< typename J >
            void operator () (const J &value) const {
                log(level, json(value));
            }
        } debug = {};

        /// The info level logger
        const struct info_tag {
            static const std::size_t level = 2000u;

            template< typename J >
            void operator () (const J &value) const {
                log(level, json(value));
            }
        } info = {};


        /// Log to a certain level
        template< typename L, typename J >
        void log( L, const J &value ) {
            log(L::level, fostlib::json(value));
        }


    }


}


#endif // FOST_LOG_HPP
