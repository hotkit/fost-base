/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include "log.hpp"

#include <fost/threadsafe-store.hpp>
#include <fost/insert.hpp>


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
    Used for adding and removing scoped sinks.
*/


std::size_t fost_base::log_proxy::tap(logging::global_sink_configuration *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::tap_global,
            boost::lambda::_1, s));
}
std::size_t fost_base::log_proxy::log_queue::tap_global(
    logging::global_sink_configuration *s
) {
    global_taps.push_back(s);
    return global_taps.size();
}
std::size_t fost_base::log_proxy::untap(logging::global_sink_configuration *s) {
    return queue.synchronous<std::size_t>(
        boost::lambda::bind(&log_queue::untap_global,
            boost::lambda::_1, s));
}
std::size_t fost_base::log_proxy::log_queue::untap_global(
    logging::global_sink_configuration *s
) {
    global_sinks_type::iterator p;
    while ( ( p = std::find(global_taps.begin(), global_taps.end(), s) ) !=
            global_taps.end() )
        global_taps.erase(p);
    return global_taps.size();
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
    typedef std::vector< boost::shared_ptr< detail::global_sink_wrapper_base > > sinks_type;
    sinks_type sinks;
    gsc_impl( const json &configuration ) {
        if ( !configuration.has_key("sinks") )
            throw exceptions::null("Configuration must have a 'sinks' key which "
                "provides a list of global sinks");
        for ( json::const_iterator sink_iter(configuration["sinks"].begin()),
                end(configuration["sinks"].end()); sink_iter != end; ++sink_iter ) {
            typedef sink_registry_type::found_t f_type;
            f_type found(g_sink_registry().find(
                coerce<string>((*sink_iter)["name"])));
            for ( f_type::const_iterator s_it(found.begin()); s_it != found.end(); ++s_it )
                sinks.push_back((*s_it)->construct((*sink_iter)["configuration"]));
        }
    }
    json description(const json &configuration) const {
        json d;
        insert(d, "configuation", configuration);
        for ( sinks_type::const_iterator s(sinks.begin()); s != sinks.end(); ++s )
            push_back(d, "sinks", (*s)->name());
        return d;
    }
};
fostlib::logging::global_sink_configuration::global_sink_configuration(
    const json &configuration )
: impl( new gsc_impl(configuration) ) {
    fost_base::log_proxy::proxy().tap(this);
    info("Started a global sink configuration",
        impl->description(configuration));
}

fostlib::logging::global_sink_configuration::~global_sink_configuration() {
    fost_base::log_proxy::proxy().untap(this);
    delete impl;
}
