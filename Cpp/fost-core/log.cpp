/*
    Copyright 2010-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include "log.hpp"
#include <fost/insert.hpp>

#include <utility>


using namespace fostlib;


namespace {
    const module c_legacy("{unkown}");
}


/*
    fostlib::logging::message
*/


fostlib::log::message::message(
    const fostlib::module &m, std::size_t l, nliteral n, const json &j
) : when(timestamp::now()), module(m), level(l), name(n), body(j) {
}


json fostlib::coercer<json, fostlib::log::message>::coerce(
    const fostlib::log::message &m
) {
    json js;
    insert(js, "when", fostlib::coerce<json>(m.when()));
    insert(js, "module", fostlib::coerce<json>(m.module()));
    insert(js, "level", "value", fostlib::coerce<json>(m.level()));
    insert(js, "level", "name", m.name());
    insert(js, "body", m.body());
    return js;
}


/*
    fostlib::log functions
*/


void fostlib::log::log(const fostlib::log::message &m) {
    fostlib::log::detail::log_proxy::proxy().log(m);
}


namespace {
    void marker() {
    }
}
void fostlib::log::flush() {
    fostlib::log::detail::log_proxy::proxy().exec(boost::bind(marker));
}


/*
    fostlib::log::log_object
*/


fostlib::log::detail::log_object::log_object(
    const module &m, std::size_t level, fostlib::nliteral name
) : part(m), level(level), name(name) {
}


fostlib::log::detail::log_object::log_object(std::size_t level, fostlib::nliteral name)
: part(c_legacy), level(level), name(name) {
}


fostlib::log::detail::log_object::log_object(log_object &&right)
: part(right.part), level(right.level), name(std::move(right.name)),
        log_message(std::move(right.log_message)) {
    right.log_message = json();
}


fostlib::log::detail::log_object::~log_object()
try {
    fostlib::log::log(part, level, name, log_message);
} catch ( ... ) {
    absorb_exception();
}
