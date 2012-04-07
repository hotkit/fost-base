/*
    Copyright 2010-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_LOG_SINKS_HPP
#define FOST_DETAIL_LOG_SINKS_HPP
#pragma once


#include <fost/json.hpp>
#include <fost/timestamp.hpp>


#include <fost/log-message.hpp>


namespace fostlib {


    namespace log {


        namespace detail {


            /// Base class for the scoped sinks
            class FOST_CORE_DECLSPEC scoped_sink_base {
                protected:
                    /// Constructor registers the sink in the current loggers
                    scoped_sink_base();
                    /// Deregister the sink. Only used by the sub-class destructor
                    void deregister();
                    /// Execute a function in the remote sink thread
                    void remote_exec(boost::function0<void> fn);
                public:
                    /// Overriden to handle log message
                    virtual bool log(const message &m) = 0;
            };


            /// Base class for the global sink wrapper
            class FOST_CORE_DECLSPEC global_sink_wrapper_base {
                public:
                    /// Construct a global sink wrapper with a given name
                    global_sink_wrapper_base(const string &name )
                    : name(name) {
                    }
                    /// Allow sub-classing to work properly
                    virtual ~global_sink_wrapper_base() {}
                    /// Store the name the sink is registered with
                    accessors<const string> name;
                    /// Pass a log message on to the underlying sink functor
                    virtual bool log(const message &m) const = 0;
            };

            /// Wraps a sink functor so it can be used as a global sink
            template<typename I>
            class global_sink_wrapper : public global_sink_wrapper_base {
                boost::scoped_ptr< I > sink;
                bool log(const message &m) const {
                    return (*sink)(m);
                }
                public:
                    /// Construct a sink wrapper
                    global_sink_wrapper(const string &name,
                        const json &configuration)
                    : global_sink_wrapper_base(name), sink( new I(configuration) ) {
                    }
            };

            /// Base class for handling registration of global sinks
            class FOST_CORE_DECLSPEC global_sink_base : boost::noncopyable {
                protected:
                    /// Registers a global sink type with the global sink registry
                    global_sink_base(const string &);
                    /// De-register a global sink type
                    virtual ~global_sink_base();

                public:
                    /// The name of the global sink type
                    accessors<const string> name;

                    /// Construct a new instance of the global sink type
                    virtual boost::shared_ptr< global_sink_wrapper_base > construct(
                        const json &configuration) const = 0;
            };


        }


    }


}


#endif // FOST_DETAIL_LOG_SINKS_HPP
