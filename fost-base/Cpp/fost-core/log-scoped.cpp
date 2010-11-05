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
