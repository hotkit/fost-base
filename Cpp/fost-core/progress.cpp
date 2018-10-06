/**
    Copyright 2013-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-core.hpp"

#include <fost/insert.hpp>
#include <fost/log.hpp>
#include <fost/progress.hpp>
#include <fost/unicode.hpp>

#include <fost/exception/file_error.hpp>

#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>


using namespace fostlib;


namespace {
    boost::recursive_mutex g_lock;
    std::set< progress* > g_progress;
    std::set< meter::weak_observer > g_observers;
}


fostlib::progress::progress(const json &meta, work_amount upto)
: now(), last(upto), meta(meta), next_send(timestamp::now()) {
    init();
}


fostlib::progress::progress(const boost::filesystem::path &file)
: now(), next_send(timestamp::now()) {
    insert(meta, "filename", file);
    try {
        int64_t bytes(coerce<int64_t>(boost::filesystem::file_size(file)));
        last = bytes;
        insert(meta, "stat", "size", "bytes", bytes);
        std::time_t modified(
            boost::filesystem::last_write_time(file));
        insert(meta, "stat", "modified",
            timestamp(boost::posix_time::from_time_t(modified)));
    } catch ( boost::filesystem::filesystem_error &e ) {
        throw fostlib::exceptions::file_error(
            e.what(), coerce<string>(file));
    }
    init();
}


void fostlib::progress::init() {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    g_progress.insert(this);
    observers = g_observers;
    update();
}


fostlib::progress::~progress() {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    std::set< progress* >::iterator p(g_progress.find(this));
    if ( p != g_progress.end() ) {
        g_progress.erase(p);
    } else {
        log::error(c_fost_base_core)
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
    const bool complete = is_complete();
    if ( now == 0 || complete || timestamp::now() > next_send ) {
        boost::recursive_mutex::scoped_lock lock(g_lock);
        for ( std::set< meter::weak_observer >::iterator
                obs(observers.begin()); obs != observers.end(); ++obs ) {
            meter::observer_ptr observer(*obs);
            if ( observer ) {
                observer->update(observer,
                    meter::reading(meta, complete, now, last));
            }
        }
        next_send = timestamp::now() + milliseconds(50);
    }
}


void fostlib::progress::observe(meter::weak_observer obs) {
    boost::recursive_mutex::scoped_lock lock(g_lock);
    g_observers.insert(obs);
    for ( std::set< progress* >::iterator p(g_progress.begin());
            p != g_progress.end(); ++p )
        (*p)->observers.insert(obs);
}

