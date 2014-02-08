/*
    Copyright 2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-datetime.hpp"
#include <fost/timer.hpp>


fostlib::timer::timer()
: started( boost::posix_time::microsec_clock::universal_time() ) {
}


double fostlib::timer::elapsed() const {
    const boost::posix_time::ptime now =
        boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration time_taken = now - started;
    return time_taken.total_microseconds() / 1000000.0;
}
