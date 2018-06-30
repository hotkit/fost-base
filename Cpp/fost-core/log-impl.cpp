/*
    Copyright 2010-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"
#include "log.hpp"
#include <fost/thread.hpp>


using namespace fostlib;


void fostlib::log::detail::log_proxy::log(fostlib::log::message m) {
    queue.asynchronous<std::size_t>([m = std::move(m)](auto &a) {
        return a.log(std::this_thread::get_id(), m);
    });
}
std::size_t fostlib::log::detail::log_queue::log(
    std::thread::id thread, const fostlib::log::message &message
) {
    bool proceed = true;
    std::size_t processed = 0;
    {
        scoped_sinks_type &sinks = scoped_taps[thread];
        typedef scoped_sinks_type::const_reverse_iterator sink_it;
        for (sink_it s(sinks.rbegin()), e(sinks.rend());
            proceed && e != s;
            ++s, ++processed)
        {
            proceed = (*s)->log(message);
        }
    }
    if ( proceed ) {
        typedef global_sinks_type::const_reverse_iterator sink_it;
        for (sink_it s(global_taps.rbegin()), e(global_taps.rend());
            proceed && s != e; ++s, ++processed)
        {
            proceed = (*s)->log(message);
        }
    }
    return processed;
}


void fostlib::log::detail::log_proxy::exec(std::function<void(void)> fn) {
    queue.synchronous<bool>([&fn](auto &a) {
        return a.exec(fn);
    });
}
bool fostlib::log::detail::log_queue::exec(std::function<void(void)> fn) {
    fn();
    return true;
}

