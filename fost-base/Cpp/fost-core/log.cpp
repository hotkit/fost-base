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
            typedef std::vector< logging::scoped_sink* > scoped_sinks_type;
            typedef std::map< boost::thread::id, scoped_sinks_type > taps_type;
            taps_type taps;
            public:
                log_queue();

                std::size_t log(boost::thread::id, const fostlib::logging::message &m);
                std::size_t tap(boost::thread::id, logging::scoped_sink*);
                std::size_t untap(boost::thread::id, logging::scoped_sink*);
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
            std::size_t tap(logging::scoped_sink*);
            std::size_t untap(logging::scoped_sink*);
    };
}


void log_proxy::log(const fostlib::logging::message &m) {
    queue.asynchronous<std::size_t>(boost::lambda::bind(&log_queue::log,
        boost::lambda::_1, boost::this_thread::get_id(), m));
}
std::size_t log_proxy::log_queue::log(
    boost::thread::id thread, const fostlib::logging::message &message
) {
    bool proceed = true;
    std::size_t processed = 0;
    scoped_sinks_type &sinks = taps[thread];
    typedef scoped_sinks_type::const_reverse_iterator sink_it;
    for (sink_it s(sinks.rbegin()); proceed && s != sinks.rend(); ++s, ++processed)
        proceed = (*s)->log(message);
    return processed;
}


std::size_t log_proxy::tap(logging::scoped_sink *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::tap,
            boost::lambda::_1, boost::this_thread::get_id(), s));
}
std::size_t log_proxy::log_queue::tap(
    boost::thread::id thread, logging::scoped_sink *s
) {
    scoped_sinks_type &sinks = taps[thread];
    sinks.push_back(s);
    return sinks.size();
}

std::size_t log_proxy::untap(logging::scoped_sink *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::untap,
            boost::lambda::_1, boost::this_thread::get_id(), s));
}
std::size_t log_proxy::log_queue::untap(
    boost::thread::id thread, logging::scoped_sink *s
) {
    scoped_sinks_type &sinks = taps[thread];
    sinks.erase(std::find(sinks.begin(), sinks.end(), s));
    return sinks.size();
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
    log_proxy::proxy().tap(this);
}
fostlib::logging::scoped_sink::~scoped_sink() {
    log_proxy::proxy().untap(this);
}


/*
    fostlib::logging functions
*/


void fostlib::logging::log(const logging::message &m) {
    log_proxy::proxy().log(m);
}


/*
    log_queue
*/

log_proxy::log_queue::log_queue() {
    queue.push_back(logging::message(
        logging::info.level(), logging::info.name(),
        json("Logging queue initialised")));
}
