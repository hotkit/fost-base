/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PROGRESS_HPP
#define FOST_PROGRESS_HPP
#pragma once


#include <fost/thread.hpp>


namespace fostlib {


    class progress;


    /// Let us see what is happening. Create in one thread then call observe() in threads you want to measure
    class meter {
    public:
        /// Used to observe progress in a single thread
        class observer;

        /// Pointer type used by client code to interact with observers
        typedef boost::shared_ptr< observer > observer_ptr;
        /// Weak pointer type used to register observers
        typedef boost::weak_ptr< observer > weak_observer;

        /// A new progress meter for an unspecified progress item
        meter();

        /// Observe progress in the current thread. Return the index of the observer
        std::size_t observe();

        /// Return the requested observer
        observer_ptr operator [] ( std::size_t index ) const;

        /// Return true if everything is complete
        bool is_complete() const;

    private:
        std::vector< observer_ptr > observers;
    };


    /// Used to observe progress in a single thread
    class meter::observer {
        friend class meter;
        friend class progress;

        bool complete;
        std::size_t upto;

        observer();

    public:
        /// Return true if the progress is complete
        bool is_complete() const;
    };


    /// Record the progress towards completing a task
    class progress {
        friend class meter;
        friend class meter::observer;

        std::size_t now, last;
        /// Add a new observer to the progress tracker
        static void observe(meter::weak_observer);

        /// The set of observers for this progress tracker
        std::set< meter::weak_observer > observers;

        /// Update the observers on the current progress if the update time has passed
        void update();

    public:
        /// Progress recording which isn't explicitly part of a larger process for up to the specified number
        progress(std::size_t upto);

        /// Allow tracking of removal of progress recorders. Not virtual as we're not going to sub-class this
        ~progress();

        /// Returns true if the current value is at least as much as the total
        bool is_complete() const  {
            return now >= last;
        }

        /// Mark one step as having been completed
        std::size_t operator ++ ();

        /// Mark a certain amount of work as having been done
        progress &operator += (std::size_t amount);

        /// Return the current value of the progress
        std::size_t current() const {
            return now;
        }
    };


}


#endif // FOST_PROGRESS_HPP
