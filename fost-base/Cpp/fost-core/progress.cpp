/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"

#include <fost/detail/unicode.hpp>
#include <fost/exception/file_error.hpp>
#include <fost/insert.hpp>
#include <fost/log.hpp>
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


fostlib::progress::progress(const boost::filesystem::wpath &file)
: now() {
    insert(meta, "filename", file);
    boost::system::error_code error;
    uintmax_t bytes(boost::filesystem::file_size(file, error));
    if ( !error ) {
        last = bytes;
        insert(meta, "stat", "size", "bytes", bytes);
        std::time_t modified(
            boost::filesystem::last_write_time(file, error));
        if ( !error ) {
            insert(meta, "stat", "modified",
                timestamp(boost::posix_time::from_time_t(modified)));
        }
        // Would ideally call progress(meta, upto), but can't until C++11
        boost::recursive_mutex::scoped_lock lock(g_lock);
        g_progress.insert(this);
        observers = g_observers;
        update();
    } else {
        throw fostlib::exceptions::file_error(
            "File not found", coerce<string>(file));
    }
}


fostlib::progress::~progress() {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    std::set< progress* >::iterator p(g_progress.find(this));
    if ( p != g_progress.end() ) {
        g_progress.erase(p);
    } else {
        log::error()
            ("function", "fostlib::progress::~progress")
            ("meta", meta)
            ("error", "Not found in progress collection");
    }
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

