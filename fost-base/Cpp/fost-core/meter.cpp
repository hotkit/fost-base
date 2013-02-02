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
    statuses.push_back(std::make_pair(null, obs));
    progress::observe(obs);
    return statuses.size() - 1;
}


bool fostlib::meter::impl::all_complete() const {
    bool complete(true);
    for ( std::vector< status >::const_iterator s(statuses.begin());
            complete && s != statuses.end(); ++s ) {
        if ( !s->first.isnull() ) {
            complete = complete && s->first.value().is_complete();
        }
    }
    return complete;
}


void fostlib::meter::impl::update(const meter::reading &) {
}


/*
 * fostlib::meter::observer
 */


fostlib::meter::observer::observer(meter::inproc ip)
: parent(ip) {
}


void fostlib::meter::observer::update(const meter::reading &r) {
    // Note that we capture 'r' by value in the closure so the const& is safe
    // on the other end
    parent->synchronous<void>(boost::lambda::bind(
        &impl::update, boost::lambda::_1, r));
}

