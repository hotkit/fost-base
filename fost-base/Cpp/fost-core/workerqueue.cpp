/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <fost/workerqueue.hpp>


namespace {
    struct worker {
        static fostlib::counter worker_count;

        worker() {
            ++worker_count;
        }

        typedef fostlib::in_process< worker > worker_type;
        typedef std::deque< boost::shared_ptr< worker_type > > workers_type;
        typedef std::deque< fostlib::workerqueue< void >::function_type > queue_type;
        typedef std::deque< fostlib::future< bool > > futures_type;

        bool exec(workers_type::value_type) const;

        static boost::mutex &g_mutex() {
            static boost::mutex m;
            return m;
        }
        static worker::workers_type &g_workers() {
            static workers_type w;
            return w;
        }
        static queue_type &g_queue() {
            static queue_type q;
            return q;
        }
        static futures_type &g_futures() {
            static futures_type f;
            return f;
        }

        static worker *make_worker() {
            return new worker;
        }
        static void execute( workers_type::value_type w ) {
            // This doesn't need a lock as the calling function already has one
            g_futures().push_back( w->asynchronous< bool >(
                boost::lambda::bind( &::worker::exec, boost::lambda::_1, w )
            ) );
        }
    };
}


fostlib::counter worker::worker_count;


fostlib::workerqueue< void >::~workerqueue() {
}


bool ::worker::exec( workers_type::value_type self ) const {
    fostlib::workerqueue< void >::function_type f;
    { // Grab the head of the work queue to work on
        boost::mutex::scoped_lock lock(g_mutex());
        if ( g_queue().empty() ) {
            // Nothing to do, just add myself back to the queue and exit
            return false;
        } else {
            f = g_queue().front();
            g_queue().pop_front();
        }
    }
    // Now that we've dropped the lock we can do the work
    f();
    return true;
}

void fostlib::workerqueue< void >::operator () ( function_type f ) {
    boost::mutex::scoped_lock lock(::worker::g_mutex());
    // Put the work onto the queue
    ::worker::g_queue().push_back( f );
    // If there is a worker available (or there aren't enough worker to fill the pool yet)
    if ( ::worker::g_workers().empty() ) {
        // If there isn't a worker available maybe we can create one
        if ( ::worker::worker_count.value() < 2 ) {
            ::worker::workers_type::value_type w( new ::worker::worker_type(
                ::worker::make_worker
            ) );
            ::worker::execute(w);
        }
    } else {
        // Get this worker to grab an item from the queue
        ::worker::workers_type::value_type w = ::worker::g_workers().front();
        ::worker::g_workers().pop_front();
    }
}

void fostlib::workerqueue< void >::operator () () {
    fostlib::future< bool > future;
    { // Take the lock and grab the next future
        boost::mutex::scoped_lock lock(::worker::g_mutex());
        if ( ::worker::g_futures().size() ) {
            // TODO This returns the first item. It should return any completed item
            future = ::worker::g_futures().front();
            ::worker::g_futures().pop_front();
        } else
            throw fostlib::exceptions::not_implemented(
                "Tried to pull more results from the queue than work items placed in it."
            );
    }
    // Now do the blocking call to wait for it to complete
    future();
}
