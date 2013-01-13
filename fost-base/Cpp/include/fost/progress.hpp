/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PROGRESS_HPP
#define FOST_PROGRESS_HPP
#pragma once


namespace fostlib {


    /// Record the progress towards completing a task
    class progress {
    public:
        /// Progress recording which isn't explicitly part of a larger process
        progress();

        /// Part of a larger process
        progress(progress &parent);
    };


    /// Let us see what is happening
    class meter {
    };


}


#endif // FOST_PROGRESS_HPP
