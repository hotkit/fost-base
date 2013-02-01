/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/progress.hpp>


using namespace fostlib;


/*
 * fostlib::meter
 */


fostlib::meter::meter()
: pimpl(new in_process<impl>(new impl)) {
}


std::size_t fostlib::meter::observe() {
    return pimpl->synchronous<std::size_t>(
        boost::lambda::bind(&impl::observe, boost::lambda::_1, pimpl));
}


bool fostlib::meter::is_complete() const {
    return pimpl->synchronous<bool>(
        boost::lambda::bind(&impl::all_complete, boost::lambda::_1));
}


/*
 * fostlib::meter::impl
 */


std::size_t fostlib::meter::impl::observe(meter::inproc ip) {
    observer_ptr obs(new observer(ip));
    observers.push_back(obs);
    progress::observe(obs);
    return observers.size() - 1;
}


bool fostlib::meter::impl::all_complete() const {
    bool complete(true);
    for ( std::vector< observer_ptr >::const_iterator o(observers.begin());
            complete && o != observers.end(); ++o ) {
        complete = complete && (*o)->is_complete();
    }
    return complete;
}


/*
 * fostlib::meter::observer
 */


fostlib::meter::observer::observer(meter::inproc ip)
: parent(ip), complete(true), upto() {
}


bool fostlib::meter::observer::is_complete() const {
    return complete;
}


void fostlib::meter::observer::add_work(std::size_t amount) {
    complete = false;
    upto += amount;
}

