/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <f5/threading/set.hpp>


namespace {
    auto &counters() {
        static f5::tsset<const fostlib::performance *> c_counters;
        return c_counters;
    }
}


fostlib::performance::performance(const module &m, const jcursor &p)
: parent(&m), path(p) {
    counters().insert_if_not_found(this);
}


fostlib::performance::~performance() {
    counters().remove_if(
        [this](auto v) {
            return v == this;
        });
}


fostlib::json fostlib::performance::current() {
    json ret;
    counters().for_each(
        [&ret](auto v) {
            const auto value = v->value();
            if ( value )
                (v->parent->as_jcursor() / v->path).insert(ret, v->value());
        });
    return ret;
}

