/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_LOG_HPP
#define FOST_LOG_HPP
#pragma once


#include <fost/json.hpp>
#include <fost/timestamp.hpp>
#include <fost/thread.hpp>


#include <fost/log-message.hpp>
#include <fost/detail/log-sinks.hpp>


namespace fostlib {


    namespace logging {


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
        /// Add a message to the logs at a given level
        FOST_CORE_DECLSPEC inline
        void log(std::size_t level, nliteral name,
                const fostlib::json &d1, const fostlib::json &d2) {
            fostlib::json data;
            push_back(data, d1);
            push_back(data, d2);
            logging::log(message(level, name, data));
        }


        /// A scoped sink is used to provide some logging capability for a short period of time
        template< typename S >
        class scoped_sink : detail::scoped_sink_base {
            boost::scoped_ptr< S > sink_object;
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
            bool( const fostlib::logging::message& ) > > scoped_sink_fn;


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


        /// Used to create a logging level
        #define FSL_DEFINE_LOGGING_LEVEL( N, value ) \
            const struct N##_level_tag { \
                static const std::size_t level() { return value; } \
                static fostlib::nliteral name() { return #N; } \
                template< typename J > void operator () (const J &v) const { \
                    fostlib::logging::log(level(), name(), \
                        fostlib::coerce<fostlib::json>(v)); \
                } \
                template< typename J1, typename J2 > \
                void operator () ( \
                    const J1 &v1, const J2 &v2 \
                ) const { \
                    fostlib::logging::log(level(), name(), \
                        fostlib::coerce<fostlib::json>(v1), \
                        fostlib::coerce<fostlib::json>(v2)); \
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
