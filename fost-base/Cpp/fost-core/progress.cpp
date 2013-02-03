/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/progress.hpp>
#include <boost/thread/recursive_mutex.hpp>


using namespace fostlib;


namespace {
    boost::recursive_mutex g_lock;
    std::set< progress* > g_progress;
    std::set< meter::weak_observer > g_observers;
}


fostlib::progress::progress(const json &meta, work_amount upto)
: now(), last(upto), meta(meta) {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    g_progress.insert(this);
    observers = g_observers;
    update();
}


fostlib::progress::~progress() {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    g_progress.erase(g_progress.find(this));
}


work_amount fostlib::progress::operator ++ () {
    ++now;
    update();
    return now;
}


progress &fostlib::progress::operator += (work_amount amount) {
    now += amount;
    update();
    return *this;
}


void fostlib::progress::update() {
    bool complete = is_complete();
    boost::recursive_mutex::scoped_lock lock(g_lock);
    for ( std::set< meter::weak_observer >::iterator obs(observers.begin());
            obs != observers.end(); ++obs ) {
        meter::observer_ptr observer(*obs);
        if ( observer ) {
            observer->update(observer, meter::reading(complete));
        }
    }
}


void fostlib::progress::observe(meter::weak_observer obs) {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    g_observers.insert(obs);
    for ( std::set< progress* >::iterator p(g_progress.begin());
            p != g_progress.end(); ++p )
        (*p)->observers.insert(obs);
}

