/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <f5/threading/map.hpp>


namespace {
    f5::tsmap<const fostlib::jcursor *, const fostlib::performance *> c_counters;
}


fostlib::performance::performance(
    const module::data &module, const string &section, const string &name
) : path(module.jc / section / name) {
}


fostlib::performance::~performance() {
}

