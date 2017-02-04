/*
    Copyright 2011-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-datetime.hpp"
#include <fost/timer.hpp>


fostlib::timer::timer()
: started(std::chrono::steady_clock::now()) {
}


double fostlib::timer::elapsed() const {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> taken = now - started;
    return taken.count();
}

