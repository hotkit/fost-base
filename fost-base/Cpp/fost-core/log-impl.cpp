/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include "log.hpp"
#include <fost/thread.hpp>


using namespace fostlib;


void fost_base::log_proxy::log(const fostlib::logging::message &m) {
    queue.asynchronous<std::size_t>(boost::lambda::bind(&log_queue::log,
        boost::lambda::_1, boost::this_thread::get_id(), m));
}
std::size_t fost_base::log_proxy::log_queue::log(
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


std::size_t fost_base::log_proxy::tap(logging::detail::scoped_sink_base *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::tap,
            boost::lambda::_1, boost::this_thread::get_id(), s));
}
std::size_t fost_base::log_proxy::log_queue::tap(
    boost::thread::id thread, logging::detail::scoped_sink_base *s
) {
    scoped_sinks_type &sinks = taps[thread];
    sinks.push_back(s);
    return sinks.size();
}

std::size_t fost_base::log_proxy::untap(logging::detail::scoped_sink_base *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::untap,
            boost::lambda::_1, boost::this_thread::get_id(), s));
}
std::size_t fost_base::log_proxy::log_queue::untap(
    boost::thread::id thread, logging::detail::scoped_sink_base *s
) {
    scoped_sinks_type &sinks = taps[thread];
    scoped_sinks_type::iterator p;
    while ( ( p = std::find(sinks.begin(), sinks.end(), s) ) != sinks.end() )
        sinks.erase(p);
    return sinks.size();
}

void fost_base::log_proxy::exec(boost::function0<void> fn) {
    queue.synchronous<bool>(boost::lambda::bind(
        &log_proxy::log_queue::exec, boost::lambda::_1, fn));
}
bool fost_base::log_proxy::log_queue::exec(boost::function0<void> fn) {
    fn();
    return true;
}

