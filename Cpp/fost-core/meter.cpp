/*
    Copyright 2013-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/progress.hpp>
#include <fost/push_back.hpp>


using namespace fostlib;


/*
 * fostlib::meter
 */


fostlib::meter::meter()
: pimpl(new in_process<impl>(new impl)) {
    pimpl->synchronous<work_amount>(
        boost::lambda::bind(&impl::observe, boost::lambda::_1, pimpl));
}


bool fostlib::meter::is_complete() const {
    return (*this)().is_complete();
}


meter::reading fostlib::meter::operator () () const {
    return pimpl->synchronous<meter::reading>(
        boost::lambda::bind(&impl::current, boost::lambda::_1));
}


/*
 * fostlib::meter::impl
 */


work_amount fostlib::meter::impl::observe(meter::inproc ip) {
    observer_ptr obs(new observer(ip));
    statuses[obs] = null;
    progress::observe(obs);
    return statuses.size() - 1;
}


meter::reading fostlib::meter::impl::current() const {
    json meta;
    bool complete(true);
    work_amount total = 0, done = 0;

    for ( statuses_type::const_iterator s(statuses.begin());
            s != statuses.end(); ++s ) {
        if ( !s->second.isnull() ) {
            push_back(meta, s->second.value().meta());
            complete = complete && s->second.value().is_complete();
            done += s->second.value().done();
            total += s->second.value().work().value(
                s->second.value().done());
        }
    }

    return reading(meta, complete, done, total);
}


void fostlib::meter::impl::update(
    meter::observer_ptr o, const meter::reading &r
) {
    statuses[o] = r;
}


/*
 * fostlib::meter::observer
 */


fostlib::meter::observer::observer(meter::inproc ip)
: parent(ip) {
}


void fostlib::meter::observer::update(
    meter::observer_ptr o, const meter::reading &r
) {
    // Note that we capture 'r' by value in the closure so the const& is safe
    // on the other end
    parent->synchronous<void>(boost::lambda::bind(
        &impl::update, boost::lambda::_1, o, r));
}

