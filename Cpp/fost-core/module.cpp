/*
    Copyright 2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/module.hpp>
#include <mutex>


const fostlib::module fostlib::c_fost("fost");
const fostlib::module fostlib::c_fost_base(c_fost, "base");
const fostlib::module fostlib::c_fost_base_core(c_fost_base, "core");


namespace {
    fostlib::string as_str(const fostlib::module * const p, const fostlib::nliteral n) {
        if ( p ) {
            if ( n[0] == '/' ) {
                return p->as_string() + n;
            } else {
                return p->as_string()  + "/" + n;
            }
        } else {
            return n;
        }
    }
    fostlib::jcursor as_jc(const fostlib::module * const p, const fostlib::nliteral n) {
        if ( p ) {
            return p->as_jcursor() / n;
        } else {
            return fostlib::jcursor(n);
        }
    }
    fostlib::json as_js(const fostlib::module * const p, const fostlib::nliteral n) {
        return fostlib::coerce<fostlib::json>(as_jc(p, n));
    }
}


fostlib::json fostlib::module::as_json() const {
    return as_js(parent, name);
}


fostlib::string fostlib::module::as_string() const {
    return as_str(parent, name);
}


fostlib::jcursor fostlib::module::as_jcursor() const {
    return as_jc(parent, name);
}

