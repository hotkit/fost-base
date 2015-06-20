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
const fostlib::module fostlib::c_fost_base(c_fost(), "base");
const fostlib::module fostlib::c_fost_base_core(c_fost_base(), "core");


namespace {
    fostlib::string as_str(const fostlib::module::data * const p, const fostlib::nliteral n) {
        if ( p ) {
            if ( n[0] == '/' ) {
                return p->str + n;
            } else {
                return p->str + "/" + n;
            }
        } else {
            return n;
        }
    }
    fostlib::jcursor as_jc(const fostlib::module::data * const p, const fostlib::nliteral n) {
        if ( p ) {
            return p->jc / n;
        } else {
            return fostlib::jcursor(n);
        }
    }
    fostlib::json as_js(const fostlib::module::data * const p, const fostlib::nliteral n) {
        return fostlib::coerce<fostlib::json>(as_jc(p, n));
    }
}


const fostlib::module::data &fostlib::module::operator () () const {
    static data d{parent, name,
        as_js(parent, name), as_jc(parent, name), as_str(parent, name)};
    return d;
}


fostlib::module::operator const fostlib::json & () const {
    return (*this)().js;
}


fostlib::module::operator const fostlib::string & () const {
    return (*this)().str;
}


fostlib::module::operator const fostlib::jcursor & () const {
    return (*this)().jc;
}

