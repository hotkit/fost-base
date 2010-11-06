/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
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


    namespace logging {


        namespace detail {


            class FOST_CORE_DECLSPEC scoped_sink_base {
                protected:
                    scoped_sink_base();
                    void deregister();
                    void remote_exec(boost::function0<void> fn);
                public:
                    virtual bool log(const message &m) = 0;
            };


            class FOST_CORE_DECLSPEC global_sink_wrapper_base {
                public:
                    global_sink_wrapper_base(const string &name )
                    : name(name) {
                    }
                    accessors<const string> name;
            };

            template<typename I>
            class global_sink_wrapper : public global_sink_wrapper_base {
                public:
                    global_sink_wrapper(const string &name,
                        const json &configuration)
                    : global_sink_wrapper_base(name) {
                    }
            };

            class FOST_CORE_DECLSPEC global_sink_base : boost::noncopyable {
                protected:
                    global_sink_base(const string &);
                    virtual ~global_sink_base();

                public:
                    accessors<const string> name;

                    virtual boost::shared_ptr< global_sink_wrapper_base > construct(
                        const json &configuration) const = 0;
            };


        }


    }


}


#endif // FOST_DETAIL_LOG_SINKS_HPP
