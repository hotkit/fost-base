/*
    Copyright 2011, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TIMER_HPP
#define FOST_TIMER_HPP
#pragma once


#include <fost/core>

#include <boost/date_time/posix_time/posix_time.hpp>


namespace fostlib {


    /// A simple timer for measuring the duration of something
    class FOST_DATETIME_DECLSPEC timer : boost::noncopyable {
        const boost::posix_time::ptime started;
    public:
        /// Construct and start the timer
        timer();

        /// The number of seconds that have elapsed
        double elapsed() const;
    };


}


#endif // FOST_TIMER_HPP
