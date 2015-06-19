/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_LOG_HPP
#define FOST_LOG_HPP
#pragma once


#include <fost/module.hpp>
#include <fost/timestamp.hpp>
#include <fost/thread.hpp>
#include <fost/insert.hpp>
#include <fost/push_back.hpp>


#include <fost/log-message.hpp>
#include <fost/detail/log-sinks.hpp>


namespace fostlib {


    namespace log {


        /// Log a logging message
        FOST_CORE_DECLSPEC
        void log(const message &);

        /// Log to a certain level
        template< typename L, typename J >
        [[deprecated("Use a method on the log level")]]
        inline void log( L level, const J &value ) {
            level(value);
        }

        /// Add a message to the logs at a given level
        inline void log(const module &m, std::size_t level, nliteral name, const json &data) {
            log::log(message(m, level, name, data));
        }
        /// Add a message to the logs at a given level
        inline void log(const module &m, std::size_t level, nliteral name, json::array_t a) {
            log::log(message(m, level, name, a));
        }
        /// Add a message to the logs at a given level
        template<typename A, typename... J> inline
        void log(const module &m, std::size_t level, nliteral name,
                 json::array_t array, const A &a, J&&...j) {
            push_back(array, a);
            log(m, level, name, std::move(array), std::forward<J>(j)...);
        }
        /// Add a message to the logs at a given level
        [[deprecated("Pass a fostlib::module instance")]]
        inline void log(std::size_t level, nliteral name, const json &data) {
            log::log(message(level, name, data));
        }
        /// Add a message to the logs at a given level for the specified module
        [[deprecated("Use the new fostlib::module class")]]
        inline void log(const string &module, std::size_t level, nliteral name,
                const json &data) {
            log::log(message(module, level, name, data));
        }
        /// Add a message to the logs at a given level
        [[deprecated("Pass a fostlib::module instance")]]
        inline void log(std::size_t level, nliteral name,
                const json &d1, const json &d2) {
            json data;
            push_back(data, d1);
            push_back(data, d2);
            log::log(message(level, name, data));
        }
        /// Add a message to the logs at a given level
        [[deprecated("Pass a fostlib::module instance")]]
        inline void log(std::size_t level, nliteral name,
                const json &d1, const json &d2,
                const json &d3) {
            json data;
            push_back(data, d1);
            push_back(data, d2);
            push_back(data, d3);
            log::log(message(level, name, data));
        }
        /// Add a message to the logs at a given level
        [[deprecated("Pass a fostlib::module instance")]]
        inline void log(std::size_t level, nliteral name,
                const json &d1, const json &d2,
                const json &d3, const json &d4) {
            json data;
            push_back(data, d1);
            push_back(data, d2);
            push_back(data, d3);
            push_back(data, d4);
            log::log(message(level, name, data));
        }

        /// Block until the current messages have all been processed
        FOST_CORE_DECLSPEC
        void flush();


        /// A scoped sink is used to provide some logging capability for a short period of time
        template< typename S >
        class scoped_sink : detail::scoped_sink_base {
            std::unique_ptr< S > sink_object;
            bool log(const message &m) {
                return (*sink_object)(m);
            }
            static void return_value(S*s, typename S::result_type &result) {
                result = (*s)();
            }
            public:
                /// The type of the sink
                typedef S sink_type;

                /// Construct a sink which doesn't require any parameters
                scoped_sink()
                : sink_object( new sink_type ) {
                }
                /// Construct a sink passing in one parameter
                template<typename A1>
                scoped_sink(const A1 &a1)
                : sink_object( new sink_type(a1) ) {
                }
                /// Make sure that the sink is destructed so log messages no longer arrive for it
                ~scoped_sink() {
                    deregister();
                }

                /// Fetch the data object from the sink
                typename sink_type::result_type operator () () {
                    typename sink_type::result_type result;
                    remote_exec(boost::lambda::bind(
                        &return_value, sink_object.get(), boost::ref(result)));
                    return result;
                }
        };
        /// Use this where you just need a logging function to act as a scoped logger
        typedef scoped_sink< boost::function<
            bool( const log::message& ) > > scoped_sink_fn;


        /// Create an instance of this class to register a global sink
        template< typename F >
        class global_sink : detail::global_sink_base {
            /// Create the logging object itself
            boost::shared_ptr< detail::global_sink_wrapper_base > construct(
                    const json &configuration) const {
                return boost::shared_ptr< detail::global_sink_wrapper_base >(
                    new detail::global_sink_wrapper< F >(name(), configuration) );
            }
            public:
                /// Create a global sink providing the configuration name
                global_sink(const string &name)
                : global_sink_base(name) {
                }

                /// The name of the sink used for configuration
                using global_sink_base::name;
        };


        namespace detail {
            class log_queue;
        }

        /// Create an instance of this to control the configuration of the global sinks
        class FOST_CORE_DECLSPEC global_sink_configuration {
            struct gsc_impl;
            gsc_impl *impl;

            friend class detail::log_queue;
            void log(const message &m);

            public:
                /// Construct a new global sink configuration for accepting logs
                global_sink_configuration(const json &configuration);
                /// Remove the configuration from the log sinks
                ~global_sink_configuration();
        };


        namespace detail {
            /// Used to make the logging of JSON objects easier
            class FOST_CORE_DECLSPEC log_object {
                /// The log level
                std::size_t level;
                /// The name of the log level
                nliteral name;
                /// The module this is part of
                module *part;
                /// The name of the module
                nullable< string > module_name;
                /// The log message being constructed
                json log_message;
            public:
                /// Start the log message
                log_object(const module &, std::size_t, nliteral);
                /// Start the log message
                log_object(std::size_t level, nliteral name);
                /// Move constructor
                log_object(log_object &&);
                /// Send the constructed log message
                ~log_object();

                /// Set the module name
                [[deprecated("Use the new fostlib::module constructor")]]
                log_object &module(const string &m) {
                    module_name = m;
                    return *this;
                }

                /// Log the value at the requested key
                template< typename P1, typename V >
                log_object &operator() (P1 p1, V v) {
                    insert(log_message, p1, coerce<json>(v));
                    return *this;
                }

                /// Log the message at the requested key path
                template< typename P1, typename P2, typename V >
                log_object &operator() (P1 p1, P2 p2, V v) {
                    insert(log_message, p1, p2, coerce<json>(v));
                    return *this;
                }
            };
        }

        /// Used to create a logging level
        #define FSL_DEFINE_LOGGING_LEVEL( N, value ) \
            const struct N##_level_tag { \
                static const std::size_t level() { return value; } \
                static fostlib::nliteral name() { return #N; } \
                detail::log_object operator() (const module &m) const { \
                    return detail::log_object(m, level(), name()); \
                 } \
                 template<typename...J> \
                 void operator () (const module &m, J&&... j) const { \
                     fostlib::log::log(m, level(), name(), json::array_t(), std::forward<J>(j)...); \
                 } \
                [[deprecated("Pass a fostlib::module instance")]] \
                detail::log_object operator() () const { \
                    return detail::log_object(level(), name()); \
                } \
                template< typename J > \
                [[deprecated("Pass a fostlib::module instance")]] \
                void operator () (const J &v) const { \
                    fostlib::log::log(level(), name(), \
                        fostlib::coerce<fostlib::json>(v)); \
                } \
                template< typename J1, typename J2 > \
                [[deprecated("Pass a fostlib::module instance")]] \
                void operator () ( \
                    const J1 &v1, const J2 &v2 \
                ) const { \
                    fostlib::log::log(level(), name(), \
                        fostlib::coerce<fostlib::json>(v1), \
                        fostlib::coerce<fostlib::json>(v2)); \
                } \
                template< typename J1, typename J2, typename J3 > \
                [[deprecated("Pass a fostlib::module instance")]] \
                void operator () ( \
                    const J1 &v1, const J2 &v2, const J3 &v3 \
                ) const { \
                    fostlib::log::log(level(), name(), \
                        fostlib::coerce<fostlib::json>(v1), \
                        fostlib::coerce<fostlib::json>(v2), \
                        fostlib::coerce<fostlib::json>(v3)); \
                } \
                template< typename J1, typename J2, \
                    typename J3, typename J4 > \
                [[deprecated("Pass a fostlib::module instance")]] \
                void operator () ( \
                    const J1 &v1, const J2 &v2, const J3 &v3, const J4 &v4 \
                ) const { \
                    fostlib::log::log(level(), name(), \
                        fostlib::coerce<fostlib::json>(v1), \
                        fostlib::coerce<fostlib::json>(v2), \
                        fostlib::coerce<fostlib::json>(v3), \
                        fostlib::coerce<fostlib::json>(v4)); \
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

        /// Can be used to choose all logging levels
        const struct all_level_tag {
            static const std::size_t level() { return 0u; }
            static nliteral name() { return "all"; }
        } all = {};


    }


}


#endif // FOST_LOG_HPP
