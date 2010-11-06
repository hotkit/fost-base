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


/*
    Used for adding and removing scoped sinks.
*/


std::size_t fost_base::log_proxy::tap(logging::detail::scoped_sink_base *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::tap_scoped,
            boost::lambda::_1, boost::this_thread::get_id(), s));
}
std::size_t fost_base::log_proxy::log_queue::tap_scoped(
    boost::thread::id thread, logging::detail::scoped_sink_base *s
) {
    scoped_sinks_type &sinks = scoped_taps[thread];
    sinks.push_back(s);
    return sinks.size();
}

std::size_t fost_base::log_proxy::untap(logging::detail::scoped_sink_base *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::untap_scoped,
            boost::lambda::_1, boost::this_thread::get_id(), s));
}
std::size_t fost_base::log_proxy::log_queue::untap_scoped(
    boost::thread::id thread, logging::detail::scoped_sink_base *s
) {
    scoped_sinks_type &sinks = scoped_taps[thread];
    scoped_sinks_type::iterator p;
    while ( ( p = std::find(sinks.begin(), sinks.end(), s) ) != sinks.end() )
        sinks.erase(p);
    return sinks.size();
}


/*
    fostlib::logging::scoped_sink
*/

fostlib::logging::detail::scoped_sink_base::scoped_sink_base() {
    fost_base::log_proxy::proxy().tap(this);
}
void fostlib::logging::detail::scoped_sink_base::deregister() {
    fost_base::log_proxy::proxy().untap(this);
}
void fostlib::logging::detail::scoped_sink_base::remote_exec(
    boost::function0<void> fn
) {
    fost_base::log_proxy::proxy().exec(fn);
}
