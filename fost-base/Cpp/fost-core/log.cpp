/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/log.hpp>
#include <fost/thread.hpp>
#include <fost/insert>

#include <deque>
#include <boost/thread/thread.hpp>


using namespace fostlib;


namespace {
    // Proxy for the actual logging object which is in another thread
    class log_proxy {
        class log_queue {
            std::deque< logging::message > queue;
            std::set< std::pair< boost::thread::id, logging::scoped_sink* > > taps;
            public:
                log_queue();

                std::size_t log(boost::thread::id, const fostlib::json &m);
                void tap(boost::thread::id, logging::scoped_sink*);
                void untap(boost::thread::id, logging::scoped_sink*);
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

            void log(const fostlib::json &j);
    };
}


void log_proxy::log(const fostlib::json &j) {
    queue.asynchronous<std::size_t>(boost::lambda::bind(&log_queue::log,
        boost::lambda::_1, boost::this_thread::get_id(), j));
}
std::size_t log_proxy::log_queue::log(
    boost::thread::id thread, const fostlib::json &message
) {
    throw exceptions::not_implemented(
        "log_proxy::log_queue::log(boost::thread::id, const fostlib::json &)");
}


/*
    fostlib::logging::message
*/


fostlib::logging::message::message(std::size_t l, nliteral n, const json &j)
: when(timestamp::now()), level(l), name(n), body(j) {
}
fostlib::logging::message::message(const string &m,
    std::size_t l, nliteral n, const json &j)
: when(timestamp::now()), module(m), level(l), name(n), body(j) {
}


json fostlib::coercer<json, logging::message>::coerce(
    const logging::message &m
) {
    json js;
    insert(js, "when", fostlib::coerce<json>(m.when()));
    insert(js, "module", m.module().isnull() ? json() : json(m.module().value()));
    insert(js, "level", "value", fostlib::coerce<json>(m.level()));
    insert(js, "level", "name", m.name());
    insert(js, "body", m.body());
    return js;
}


/*
    fostlib::logging::scoped_sink
*/

fostlib::logging::scoped_sink::scoped_sink() {
    throw fostlib::exceptions::not_implemented(
        "fostlib::logging::scoped_sink::scoped_sink()");
}

fostlib::logging::scoped_sink::~scoped_sink() {
}


/*
    fostlib::logging functions
*/


void fostlib::logging::log(const logging::message &m) {
    log_proxy::proxy().log(coerce<json>(m));
}


/*
    log_queue
*/

log_proxy::log_queue::log_queue() {
    queue.push_back(logging::message(
        logging::info.level(), logging::info.name(),
        json("Logging queue initialised")));
}
