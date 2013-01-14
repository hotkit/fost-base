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


fostlib::meter::meter() {
}


std::size_t fostlib::meter::observe() {
    observer_ptr obs(new observer);
    observers.push_back(obs);
    progress::observe(obs);
    return observers.size() - 1;
}


fostlib::meter::observer_ptr fostlib::meter::operator [] ( std::size_t index ) const {
    // We really want the bounds checking here
    return observers.at(index);
}


bool fostlib::meter::is_complete() const {
    return false;
}


/*
 * fostlib::meter::observer
 */


bool fostlib::meter::observer::is_complete() const {
    return false;
}

