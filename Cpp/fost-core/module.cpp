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
    using lock_type = std::lock_guard<std::recursive_mutex>;
}


const fostlib::module fostlib::c_fost("fost");
const fostlib::module fostlib::c_fost_base(c_fost, "base");
const fostlib::module fostlib::c_fost_base_core(c_fost_base, "core");


fostlib::module::module(const string &n)
: parent(nullptr), name(n) {
}


fostlib::module::module(const module &m, const string &n)
: parent(&m), name(n) {
}


fostlib::module::operator const fostlib::string & () const {
    lock_type lock(g_mutex);
    if ( str.empty() ) {
        if ( parent ) {
            if ( name[0] == '/' ) {
                str = static_cast<const string &>(*parent) + name;
            } else {
                str = static_cast<const string &>(*parent) + "/" + name;
            }
        } else {
            str = name;
        }
    }
    return str;
}


fostlib::module::operator const fostlib::jcursor & () const {
    lock_type lock(g_mutex);
    if ( !jc.size() ) {
        if ( parent ) {
            jc = static_cast<const jcursor &>(*parent) / name;
        } else {
            jc = jcursor(name);
        }
    }
    return jc;
}

