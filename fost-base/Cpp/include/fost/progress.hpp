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
        std::size_t now, last;
    public:
        /// Progress recording which isn't explicitly part of a larger process for up to the specified number
        progress(std::size_t upto);

        /// Returns true if the current value is at least as much as the total
        bool is_complete() const  {
            return now >= last;
        }

        /// Mark one step as having been completed
        std::size_t operator ++ ();

        /// Return the current value of the progress
        std::size_t current() const {
            return now;
        }
    };


    /// Let us see what is happening
    class meter {
    };


}


#endif // FOST_PROGRESS_HPP
