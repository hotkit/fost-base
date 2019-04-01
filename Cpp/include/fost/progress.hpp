/**
    Copyright 2013-2019, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_PROGRESS_HPP
#define FOST_PROGRESS_HPP
#pragma once


#include <fost/filesystem.hpp>
#include <fost/timestamp.hpp>
#include <fost/thread.hpp>

#include <set>


namespace fostlib {


    class progress;


    /// The type of the work measurement
    typedef uint64_t work_amount;


    /// Let us see what is happening. Create in one thread then call observe()
    /// in threads you want to measure
    class FOST_CORE_DECLSPEC meter {
      public:
        /// Used to observe progress in a single thread
        class observer;

        /// Pointer type used by client code to interact with observers
        typedef boost::shared_ptr<observer> observer_ptr;
        /// Weak pointer type used to register observers
        typedef boost::weak_ptr<observer> weak_observer;

        /// A new progress meter for an unspecified progress item
        meter();

        /// A reading
        class reading {
            timestamp captured;

          public:
            /// Allow default constructing
            reading() : captured(timestamp::now()), is_complete(true) {}
            /// A reading with a given value
            reading(bool c) : captured(timestamp::now()), is_complete(c) {}
            /// A reading for a certain amount of work
            reading(json m, bool c, work_amount done, work_amount outof)
            : captured(timestamp::now()),
              is_complete(c),
              work(outof),
              done(done),
              meta(m) {}

            /// The time that the reading was taken
            timestamp when() const { return captured; }

            /// Determine whether the progress is complete
            accessors<bool> is_complete;
            /// The amount of work we know needs to be done
            accessors<nullable<work_amount>> work;
            /// The amount of work we know has been done
            accessors<work_amount> done;
            /// Information about what we are processing
            accessors<json> meta;
        };

        /// Return true if everything is complete
        bool is_complete() const;

        /// Return a reading showing the aggregate work
        reading operator()() const;

      private:
        class impl;
        typedef boost::shared_ptr<in_process<impl>> inproc;
        /// Implement a remote thread for handling progress updates
        class impl {
          public:
            /// Use this inproc to start to observe change in progress
            work_amount observe(inproc);

            /// Used by the observer to send a new reading
            void update(observer_ptr, const reading &);

            /// Return whether all of the observers are complete or not
            meter::reading current() const;

          private:
            /// The type of the observation statuses
            typedef std::map<observer_ptr, nullable<reading>> statuses_type;
            /// Observers owned by the meter, but never to be directly accessed
            /// by it (they are owned by the impl thread, but run in the
            /// progress thread)
            statuses_type statuses;
        };
        inproc pimpl;
    };


    /// Used to observe progress within the progress thread
    class meter::observer {
        friend class meter;

        meter::inproc parent;

        observer(meter::inproc);

      public:
        /// Add knowledge about more work that's needed
        void update(meter::observer_ptr, const meter::reading &);
    };


    /// Record the progress towards completing a task
    class FOST_CORE_DECLSPEC progress {
        friend class meter;
        friend class meter::observer;

        work_amount now, last;
        /// Add a new observer to the progress tracker
        static void observe(meter::weak_observer);

        /// The set of observers for this progress tracker
        std::set<meter::weak_observer> observers;

        /// Update the observers on the current progress if the update time has
        /// passed
        void update();

        /// Meta data about the work being done
        json meta;

        /// When we will next try to send an update to the observers
        timestamp next_send;

        /// Common initialisation code
        void init();

      public:
        /// Progress recording which isn't explicitly part of a larger process
        /// for up to the specified number
        progress(const json &meta, work_amount upto);

        /// Progress recording for a file content
        progress(const boost::filesystem::path &file);

        /// Allow tracking of removal of progress recorders. Not virtual as
        /// we're not going to sub-class this
        ~progress();

        /// Returns true if the current value is at least as much as the total
        bool is_complete() const { return now >= last; }

        /// Mark one step as having been completed
        work_amount operator++();

        /// Mark a certain amount of work as having been done
        progress &operator+=(work_amount amount);

        /// Return the current value of the progress
        work_amount current() const { return now; }

        /// The amount of work to be done
        work_amount total() const { return last; }
    };


}


#endif // FOST_PROGRESS_HPP
