/*
    Copyright 1997-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <fost/thread.hpp>

#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>


using namespace fostlib;


namespace {


    counter &g_workers() {
        // We have to leak this or the decrement when threads get cleaned
        // can easily run after this is destructed.
        static counter *c = new counter;
        return *c;
    }


}


/*
    fostlib::worker
*/


fostlib::worker::worker()
: m_terminate( false ), m_thread( boost::bind( &worker::execute, this ) ) {
    ++g_workers();
}


fostlib::worker::~worker()
try {
    {
        boost::mutex::scoped_lock lock( m_mutex );
        m_terminate = true;
        m_control.notify_all();
    }
    m_thread.join();
    --g_workers();
} catch ( ... ) {
    absorb_exception();
}


boost::shared_ptr< fostlib::detail::future_result< void > > fostlib::worker::operator()
        ( boost::function0< void > f ) {
    boost::shared_ptr< detail::future_result< void > > future( new detail::future_result< void > );
    queue( future, f );
    return future;
}


 void fostlib::worker::queue(
     boost::shared_ptr< detail::future_result< void > > future, boost::function0< void > f
 ) const {
    boost::mutex::scoped_lock lock( m_mutex );
    m_queue.push_back( std::make_pair( future, f ) );
    m_control.notify_all();
}


void fostlib::worker::execute() {
    fostlib::exceptions::structured_handler handler;
#ifdef FOST_OS_WINDOWS
    com_hr( ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED ), L"CoInitializeEx at start of fostlib::worker thread" );
#endif
    bool terminate;
    do {
        t_queue job;
        { // Find a job to perform
            boost::mutex::scoped_lock lock( m_mutex );
            terminate = m_terminate;
            if ( !terminate && m_queue.empty() )
                m_control.wait( lock );
            job.swap( m_queue );
        }
        for ( t_queue::const_iterator j( job.begin() ); j != job.end(); ++j ) {
            // Execute job
            try {
                const t_queue::value_type &job = *j;
                if ( terminate )
                    job.first->m_exception = L"Thread terminated";
                else
                    job.second();
            } catch ( fostlib::exceptions::exception &e ) {
                boost::mutex::scoped_lock lock( j->first->m_mutex );
                j->first->m_exception = coerce< fostlib::string >( e );
            } catch ( ... ) {
                boost::mutex::scoped_lock lock( j->first->m_mutex );
                j->first->m_exception = L"An unknown exception was caught";
                terminate = true; // Kill the thread after an unknown exception
            }
            {// Notify futures
                boost::mutex::scoped_lock lock( j->first->m_mutex );
                j->first->m_completed = true;
                j->first->m_has_result.notify_all();
            }
            if ( !terminate ) {
                boost::mutex::scoped_lock lock( m_mutex );
                terminate = m_terminate;
            }
        }
    } while ( !terminate );
}


int fostlib::worker::workers() {
    return g_workers().value();
}


/*
    fostlib::detail::future_result< void >
*/


fostlib::detail::future_result< void >::future_result()
: m_completed( false ) {
}


fostlib::detail::future_result< void >::~future_result() {
}


fostlib::nullable< fostlib::string > fostlib::detail::future_result< void >::exception() {
    boost::mutex::scoped_lock lock( m_mutex );
    if ( !this->completed() )
        m_has_result.wait( lock, boost::lambda::var( m_completed ) );
    return m_exception;
}


void fostlib::detail::future_result< void >::wait() {
    fostlib::nullable< fostlib::string > e( exception() );
    if ( !e.isnull() )
        throw fostlib::exceptions::forwarded_exception( e.value() );
}


void fostlib::detail::future_result< void >::wait(const timediff &td) {
    boost::mutex::scoped_lock lock( m_mutex );
    if ( !this->completed() )
        m_has_result.timed_wait( lock, td, boost::lambda::var( m_completed ) );
}


#ifdef FOST_OS_WINDOWS
    #include "thread-win.cpp"
#else
    #include "thread-linux.cpp"
#endif

