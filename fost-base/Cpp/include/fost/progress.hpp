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

        /// Mark a certain amount of work as having been done
        std::size_t operator += (std::size_t amount);

        /// Return the current value of the progress
        std::size_t current() const {
            return now;
        }
    };


    /// Let us see what is happening. Create in one thread then call observe() in threads you want to measure
    class meter {
    public:
        /// Used to observe progress in a single thread
        class observer;

        /// Pointer type used by client code to interact with observers
        typedef boost::shared_ptr< observer > observer_ptr;

        /// A new progress meter for an unspecified progress item
        meter();

        /// Observe progress in the current thread. Return the index of the observer
        std::size_t observe();

        /// Return the requested observer
        observer_ptr operator [] ( std::size_t index ) const;

    private:
        std::vector< observer_ptr > observers;
    };


    /// Used to observe progress in a single thread
    class meter::observer {
    public:
        /// Return true if the progress is complete
        bool is_complete() const;
    };


}


#endif // FOST_PROGRESS_HPP
