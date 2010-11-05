/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include "log.hpp"
#include <fost/insert>


using namespace fostlib;


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
    if ( !m.module().isnull() )
        insert(js, "module", json(m.module().value()));
    insert(js, "level", "value", fostlib::coerce<json>(m.level()));
    insert(js, "level", "name", m.name());
    insert(js, "body", m.body());
    return js;
}


/*
    fostlib::logging functions
*/


void fostlib::logging::log(const logging::message &m) {
    fost_base::log_proxy::proxy().log(m);
}
