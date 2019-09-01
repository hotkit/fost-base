/**
    Copyright 2010-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-core.hpp"
#include "log.hpp"

#include <fost/threadsafe-store.hpp>
#include <fost/insert.hpp>


using namespace fostlib;


namespace {
    typedef threadsafe_store<fostlib::log::detail::global_sink_base *>
            sink_registry_type;
    sink_registry_type &g_sink_registry() {
        static sink_registry_type s;
        return s;
    }
}


/*
    Used for adding and removing scoped sinks.
*/


std::size_t fostlib::log::detail::log_proxy::tap(
        fostlib::log::global_sink_configuration *s) {
    return queue.synchronous<std::size_t>(
            [s](auto &a) { return a.tap_global(s); });
}
std::size_t fostlib::log::detail::log_queue::tap_global(
        fostlib::log::global_sink_configuration *s) {
    global_taps.push_back(s);
    return global_taps.size();
}
std::size_t fostlib::log::detail::log_proxy::untap(
        fostlib::log::global_sink_configuration *s) {
    return queue.synchronous<std::size_t>(
            [s](auto &a) { return a.untap_global(s); });
}
std::size_t fostlib::log::detail::log_queue::untap_global(
        fostlib::log::global_sink_configuration *s) {
    global_sinks_type::iterator p;
    while ((p = std::find(global_taps.begin(), global_taps.end(), s))
           != global_taps.end())
        global_taps.erase(p);
    return global_taps.size();
}


/*
    fostlib::log::detail::global_sink_base
*/


fostlib::log::detail::global_sink_base::global_sink_base(const string &n)
: name(n) {
    g_sink_registry().add(name(), this);
}
fostlib::log::detail::global_sink_base::~global_sink_base() {
    g_sink_registry().remove(name(), this);
}


/*
    fostlib::log::global_sink_configuration
*/


struct fostlib::log::global_sink_configuration::gsc_impl {
    typedef std::vector<boost::shared_ptr<detail::global_sink_wrapper_base>>
            sinks_type;
    sinks_type sinks;
    gsc_impl(const json &configuration) {
        if (!configuration.has_key("sinks"))
            throw exceptions::null(
                    "Configuration must have a 'sinks' key which "
                    "provides a list of global sinks");
        for (json::const_iterator sink_iter(configuration["sinks"].begin()),
             end(configuration["sinks"].end());
             sink_iter != end; ++sink_iter) {
            typedef sink_registry_type::found_t f_type;
            f_type found(g_sink_registry().find(
                    coerce<string>((*sink_iter)["name"])));
            for (f_type::const_iterator s_it(found.begin());
                 s_it != found.end(); ++s_it) {
                sinks.push_back(
                        (*s_it)->construct((*sink_iter)["configuration"]));
            }
        }
    }
    json description(const json &configuration) const {
        json d;
        insert(d, "configuration", configuration);
        for (sinks_type::const_iterator s(sinks.begin()); s != sinks.end(); ++s)
            push_back(d, "sinks", (*s)->name());
        return d;
    }

    bool log(const fostlib::log::message &m) {
        bool p = true;
        for (sinks_type::const_iterator s(sinks.begin()); p && s != sinks.end();
             ++s)
            p = (*s)->log(m);
        return p;
    }
};
fostlib::log::global_sink_configuration::global_sink_configuration(
        const json &configuration)
: impl(new gsc_impl(configuration)) {
    fostlib::log::detail::log_proxy::proxy().tap(this);
    info(c_fost_base_core, "Started a global sink configuration",
         impl->description(configuration));
}


fostlib::log::global_sink_configuration::~global_sink_configuration() {
    fostlib::log::detail::log_proxy::proxy().untap(this);
    delete impl;
}


bool fostlib::log::global_sink_configuration::log(const message &m) {
    return impl->log(m);
}
