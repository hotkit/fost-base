/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/log.hpp>
#include <fost/thread.hpp>

#include <deque>


using namespace fostlib;


/*
    fostlib::logging::message
*/


fostlib::logging::message::message(std::size_t l, nliteral n, const json &j)
: when(timestamp::now()), level(l), name(n), body(j) {
}


/*
    fostlib::logging functions
*/


namespace {
    class log_proxy {
        class log_queue {
            std::deque< logging::message > queue;
            public:
                log_queue();
        };
        in_process< log_queue > queue;

        log_proxy()
        : queue( new log_queue ) {
        }

        public:
            static const log_proxy &proxy() {
                static log_proxy p;
                return p;
            }
    };
}


void fostlib::logging::log(const logging::message &) {
    log_proxy::proxy();
}


/*
    log_queue
*/

log_proxy::log_queue::log_queue() {
    queue.push_back(logging::message(
        logging::info.level(), logging::info.name(),
        json("Logging queue initialised")));
}
