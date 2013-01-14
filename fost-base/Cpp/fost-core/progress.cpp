/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/progress.hpp>


using namespace fostlib;


namespace {
    boost::mutex g_lock;
    std::set< progress* > g_progress;
    std::set< meter::weak_observer > g_observers;
}


fostlib::progress::progress(std::size_t upto)
: now(), last(upto) {
    boost::mutex::scoped_lock lock(g_lock);
    g_progress.insert(this);
    observers = g_observers;
}


fostlib::progress::~progress() {
    boost::mutex::scoped_lock lock(g_lock);
    g_progress.erase(g_progress.find(this));
}


std::size_t fostlib::progress::operator ++ () {
    return ++now;
}


progress &fostlib::progress::operator += (std::size_t amount) {
    now += amount;
    return *this;
}


void fostlib::progress::observe(meter::weak_observer obs) {
    boost::mutex::scoped_lock lock(g_lock);
    g_observers.insert(obs);
    for ( std::set< progress* >::iterator p(g_progress.begin());
            p != g_progress.end(); ++p )
        (*p)->observers.insert(obs);
}

