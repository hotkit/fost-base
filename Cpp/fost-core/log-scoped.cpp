/*
    Copyright 2010-2017, Felspar Co Ltd. http://support.felspar.com/
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


std::size_t fostlib::log::detail::log_proxy::tap(
    fostlib::log::detail::scoped_sink_base *s
) {
    return queue.synchronous<std::size_t>(
        [s](auto &a) {
            return a.tap_scoped(std::this_thread::get_id(), s);
        });
}
std::size_t fostlib::log::detail::log_queue::tap_scoped(
    std::thread::id thread, fostlib::log::detail::scoped_sink_base *s
) {
    scoped_sinks_type &sinks = scoped_taps[thread];
    sinks.push_back(s);
    return sinks.size();
}

std::size_t fostlib::log::detail::log_proxy::untap(
    fostlib::log::detail::scoped_sink_base *s
) {
    return queue.synchronous<std::size_t>(
        [s](auto &a) {
            return a.untap_scoped(std::this_thread::get_id(), s);
        });
}
std::size_t fostlib::log::detail::log_queue::untap_scoped(
    std::thread::id thread, fostlib::log::detail::scoped_sink_base *s
) {
    scoped_sinks_type &sinks = scoped_taps[thread];
    scoped_sinks_type::iterator p;
    while ( ( p = std::find(sinks.begin(), sinks.end(), s) ) != sinks.end() )
        sinks.erase(p);
    return sinks.size();
}


/*
    fostlib::log::scoped_sink
*/

fostlib::log::detail::scoped_sink_base::scoped_sink_base() {
    fostlib::log::detail::log_proxy::proxy().tap(this);
}
void fostlib::log::detail::scoped_sink_base::deregister() {
    fostlib::log::detail::log_proxy::proxy().untap(this);
}
void fostlib::log::detail::scoped_sink_base::remote_exec(
    std::function<void(void)> fn
) {
    fostlib::log::detail::log_proxy::proxy().exec(fn);
}

