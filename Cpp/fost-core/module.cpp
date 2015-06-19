/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/module.hpp>
#include <mutex>


namespace {
    std::recursive_mutex g_mutex;
}


const fostlib::module fostlib::c_fost("fost");
const fostlib::module fostlib::c_fost_base(c_fost, "base");
const fostlib::module fostlib::c_fost_base_core(c_fost_base, "core");


fostlib::module::module(const string &n)
: name(n) {
}


fostlib::module::module(const module &m, const string &n)
: parent(&m), name(n) {
}

