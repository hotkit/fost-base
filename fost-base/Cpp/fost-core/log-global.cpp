/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include "log.hpp"
#include <fost/threadsafe-store.hpp>


using namespace fostlib;


namespace {
    typedef threadsafe_store< logging::detail::global_sink_base* >
        sink_registry_type;
    sink_registry_type &g_sink_registry() {
        static sink_registry_type s;
        return s;
    }
}


/*
    fostlib::logging::detail::global_sink_base
*/


fostlib::logging::detail::global_sink_base::global_sink_base(const string &n)
: name(n) {
    g_sink_registry().add(name(), this);
}
fostlib::logging::detail::global_sink_base::~global_sink_base() {
    g_sink_registry().remove(name(), this);
}


/*
    fostlib::logging::global_sink_configuration
*/


struct fostlib::logging::global_sink_configuration::gsc_impl {
    gsc_impl( const fostlib::json &configuration ) {
    }
};
fostlib::logging::global_sink_configuration::global_sink_configuration(
    const fostlib::json &configuration )
: impl( new gsc_impl(configuration) ) {
    info("Started a global sink configuration", configuration);
}

fostlib::logging::global_sink_configuration::~global_sink_configuration() {
    delete impl;
}
