/**
    Copyright 2010-2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
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
#include <fost/log-sinks.hpp>

#include <type_traits>


namespace fostlib {


    namespace log {


        /// Log a logging message
        FOST_CORE_DECLSPEC
        void log(message);

        /// Add a message to the logs at a given level
        inline void
                log(const module &m,
                    std::size_t level,
                    f5::u8string name,
                    const json &data) {
            log::log(message(m, level, name, data));
        }
        /// Add a message to the logs at a given level
        inline void
                log(const module &m,
                    std::size_t level,
                    f5::u8string name,
                    json::array_t a) {
            log::log(message(m, level, name, a));
        }
        /// Add a message to the logs at a given level
        template<typename A, typename... J>
        inline void
                log(const module &m,
                    std::size_t level,
                    f5::u8string name,
                    json::array_t array,
                    const A &a,
                    J &&... j) {
            push_back(array, fostlib::coerce<fostlib::json>(a));
            log(m, level, name, std::move(array), std::forward<J>(j)...);
        }
        /// Block until the current messages have all been processed
        FOST_CORE_DECLSPEC
        void flush();


        /// A scoped sink is used to provide some logging capability for a short
        /// period of time
        template<typename S>
        class scoped_sink : detail::scoped_sink_base {
            std::unique_ptr<S> sink_object;
            bool log(const message &m) override { return (*sink_object)(m); }
            static void return_value(S *s, typename S::result_type &result) {
                result = (*s)();
            }

          public:
            /// The type of the sink
            typedef S sink_type;

            /// Construct a sink which doesn't require any parameters
            scoped_sink() : sink_object(new sink_type) {}
            /// Construct a sink passing in one parameter
            template<typename A1>
            scoped_sink(const A1 &a1) : sink_object(new sink_type(a1)) {}
            /// Make sure that the sink is destructed so log messages no longer
            /// arrive for it
            ~scoped_sink() { deregister(); }

            /// Fetch the data object from the sink
            typename sink_type::result_type operator()() {
                typename sink_type::result_type result;
                remote_exec([this, &result]() {
                    return_value(sink_object.get(), result);
                });
                return result;
            }
        };
        /// Use this where you just need a logging function to act as a scoped
        /// logger
        using scoped_sink_fn =
                scoped_sink<std::function<bool(const log::message &)>>;


        /// Create an instance of this class to register a global sink
        template<typename F>
        class global_sink : detail::global_sink_base {
            /// Create the logging object itself
            boost::shared_ptr<detail::global_sink_wrapper_base>
                    construct(const json &configuration) const {
                return boost::shared_ptr<detail::global_sink_wrapper_base>(
                        new detail::global_sink_wrapper<F>(
                                name(), configuration));
            }

          public:
            /// Create a global sink providing the configuration name
            global_sink(const string &name) : global_sink_base(name) {}

            /// The name of the sink used for configuration
            using global_sink_base::name;
        };


        namespace detail {
            class log_queue;
        }

        /// Create an instance of this to control the configuration of the
        /// global sinks
        class FOST_CORE_DECLSPEC global_sink_configuration {
            struct gsc_impl;
            gsc_impl *impl;

            friend class detail::log_queue;
            bool log(const message &m);

          public:
            /// Construct a new global sink configuration for accepting logs
            global_sink_configuration(const json &configuration);
            /// Remove the configuration from the log sinks
            ~global_sink_configuration();
        };


        namespace detail {
            /// Used to make the logging of JSON objects easier
            class FOST_CORE_DECLSPEC log_object {
                /// The module this is part of
                const module &part;
                /// The log level
                std::size_t level;
                /// The name of the log level
                f5::u8string name;
                /// The log message being constructed
                json::object_t log_message;

              public:
                /// Start the log message
                log_object(const module &, std::size_t, f5::u8string);
                /// Move constructor
                log_object(log_object &&);
                /// Send the constructed log message
                ~log_object();

                /// Log the value at the requested key
                template<typename P1, typename V>
                log_object &operator()(const P1 &p1, V &&v) {
                    log_message[p1] = coerce<json>(std::forward<V>(v));
                    return *this;
                }

                /// Log the message at the requested key path
                template<typename P1, typename P2, typename... P>
                log_object &operator()(const P1 &p1, P2 &&p2, P &&... p) {
                    insert(log_message[p1], std::forward<P2>(p2),
                           std::forward<P>(p)...);
                    return *this;
                }
            };
        }

/// Used to create a logging level
#define FSL_DEFINE_LOGGING_LEVEL(N, value) \
    const struct N##_level_tag { \
        static std::size_t level() { return value; } \
        static f5::u8string name() { return #N; } \
        fostlib::log::detail::log_object \
                operator()(const fostlib::module &m) const { \
            return fostlib::log::detail::log_object(m, level(), name()); \
        } \
        void operator()(const fostlib::module &m, const char *msg) const { \
            fostlib::log::log(m, level(), name(), fostlib::json(msg)); \
        } \
        template<typename J> \
        void operator()(const fostlib::module &m, J &&j) const { \
            fostlib::log::log( \
                    m, level(), name(), \
                    fostlib::coerce<fostlib::json>(std::forward<J>(j))); \
        } \
        template<typename F, typename... J> \
        void operator()(const fostlib::module &m, F &&f, J &&... j) const { \
            fostlib::log::log( \
                    m, level(), name(), fostlib::json::array_t(), \
                    std::forward<F>(f), std::forward<J>(j)...); \
        } \
    } N = {}

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
            static std::size_t level() { return 0u; }
            static f5::u8string name() { return "all"; }
        } all = {};


    }


}


#endif // FOST_LOG_HPP
