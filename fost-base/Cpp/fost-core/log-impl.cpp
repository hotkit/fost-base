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


void fostlib::logging::detail::log_proxy::log(const fostlib::logging::message &m) {
    queue.asynchronous<std::size_t>(boost::lambda::bind(&log_queue::log,
        boost::lambda::_1, boost::this_thread::get_id(), m));
}
std::size_t fostlib::logging::detail::log_queue::log(
    boost::thread::id thread, const fostlib::logging::message &message
) {
    bool proceed = true;
    std::size_t processed = 0;
    {
        scoped_sinks_type &sinks = scoped_taps[thread];
        typedef scoped_sinks_type::const_reverse_iterator sink_it;
        for (sink_it s(sinks.rbegin());
                proceed && s != sinks.rend();
                ++s, ++processed)
            proceed = (*s)->log(message);
    }
    if ( proceed ) {
        typedef global_sinks_type::const_reverse_iterator sink_it;
        for (sink_it s(global_taps.rbegin());
                s != global_taps.rend();
                ++s, ++processed)
            (*s)->log(message);
    }
    return processed;
}


void fostlib::logging::detail::log_proxy::exec(boost::function0<void> fn) {
    queue.synchronous<bool>(boost::lambda::bind(
        &log_queue::exec, boost::lambda::_1, fn));
}
bool fostlib::logging::detail::log_queue::exec(
    boost::function0<void> fn
) {
    fn();
    return true;
}

