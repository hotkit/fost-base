/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/log.hpp>

#include <deque>


namespace fost_base {
    using namespace fostlib;

    // Proxy for the actual logging object which is in another thread
    class log_proxy {
        class log_queue {
            std::deque< logging::message > queue;
            typedef std::vector< logging::detail::scoped_sink_base* >
                scoped_sinks_type;
            typedef std::map< boost::thread::id, scoped_sinks_type > taps_type;
            taps_type taps;
            public:
                log_queue();

                std::size_t log(boost::thread::id, const fostlib::logging::message &m);
                std::size_t tap(boost::thread::id, logging::detail::scoped_sink_base*);
                std::size_t untap(
                    boost::thread::id, logging::detail::scoped_sink_base*);

                bool exec(boost::function0<void> fn);
        };
        in_process< log_queue > queue;

        log_proxy()
        : queue( new log_queue ) {
        }

        public:
            static log_proxy &proxy() {
                static log_proxy p;
                return p;
            }

            void log(const fostlib::logging::message &m);
            std::size_t tap(logging::detail::scoped_sink_base*);
            std::size_t untap(logging::detail::scoped_sink_base*);
            void exec(boost::function0<void> fn);
    };
}

