/*
    Copyright 1997-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/counter.hpp>
#include <fost/thread.hpp>
#include <fost/log.hpp>

#include <boost/lambda/lambda.hpp>

#include <future>
#include <utility>


using namespace fostlib;


namespace {


    performance p_created(c_fost_base_core, "fostlib::thread", "created");
    performance p_destroyed(c_fost_base_core, "fostlib::thread", "joined");


}


struct fostlib::worker::context {
    context();

    using t_queue =
        std::list<std::pair<
            std::shared_ptr<detail::future_result<void >>,
            std::function<void(void)>>>;
    t_queue m_queue;

    bool m_terminate;

    boost::mutex m_mutex;
    boost::condition m_control;
    std::unique_ptr<boost::thread> m_thread;

    static void execute(std::shared_ptr<context> self);
};


/*
    fostlib::worker
*/


fostlib::worker::worker()
: self(new context) {
    self->m_thread.reset(
        new boost::thread([this]() {context::execute(self);}));
    ++p_created;
}


fostlib::worker::~worker() throw ()
try {
    {
        boost::mutex::scoped_lock lock(self->m_mutex);
        self->m_terminate = true;
        self->m_control.notify_all();
    }
    if ( boost::this_thread::get_id() != self->m_thread->get_id() ) {
        /**
            It is possible for a thread to commit suicide, in which case
            there is nothing we want to notify and we certainly don't want
            to join it.

            It is far more usual that the thread is destructed from another
            thread (most often the one that created it). In this case we do
            want to shut the thread down normally.
        */
        self->m_thread->join();
    }
    ++p_destroyed;
} catch ( ... ) {
    absorb_exception();
}


std::shared_ptr< fostlib::detail::future_result< void > > fostlib::worker::operator() (
    std::function<void(void)> f
) {
    std::shared_ptr<detail::future_result<void>> future(new detail::future_result<void>);
    queue(future, f);
    return future;
}


 void fostlib::worker::queue(
     std::shared_ptr< detail::future_result< void > > future, std::function<void(void)> f
 ) const {
    boost::mutex::scoped_lock lock(self->m_mutex);
    self->m_queue.push_back(std::make_pair(future, f));
    self->m_control.notify_all();
}


int fostlib::worker::workers() {
    return p_created.value() - p_destroyed.value();
}


/*
    fostlib::worker::context
*/


fostlib::worker::context::context()
: m_terminate(false) {
}


void fostlib::worker::context::execute(std::shared_ptr<context> self) {
    fostlib::exceptions::structured_handler handler;
#ifdef FOST_OS_WINDOWS
    com_hr( ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED ), L"CoInitializeEx at start of fostlib::worker thread" );
#endif
    bool terminate = false;
    do {
        t_queue job;
        { // Find a job to perform
            boost::mutex::scoped_lock lock(self->m_mutex);
            terminate = self->m_terminate;
            if ( !terminate && self->m_queue.empty() ) {
                self->m_control.wait(lock);
            }
            job.swap(self->m_queue);
        }
        for ( t_queue::const_iterator j(job.begin()); j != job.end(); ++j ) {
            // Execute job
            try {
                const t_queue::value_type &job = *j;
                if ( terminate ) {
                    exceptions::not_implemented error(__FUNCTION__,
                            "Thread terminated -- don't have proper exception type yet");
#ifdef FOST_NO_STD_EXCEPTION_PTR
                    job.first->m_exception = coerce<json>(error);
#else
                    job.first->m_exception = std::make_exception_ptr(error);
#endif
                } else {
                    job.second();
                }
            } catch ( fostlib::exceptions::exception &e ) {
                insert(e.data(), "across-thread", true);
                boost::mutex::scoped_lock lock(j->first->m_mutex);
#ifdef FOST_NO_STD_EXCEPTION_PTR
                j->first->m_exception = coerce<json>(e);
#else
                j->first->m_exception = std::current_exception();
#endif
            } catch ( std::exception &e ) {
                boost::mutex::scoped_lock lock(j->first->m_mutex);
#ifdef FOST_NO_STD_EXCEPTION_PTR
                j->first->m_exception = coerce<string>(e.what());
#else
                j->first->m_exception = std::current_exception();
#endif
            } catch ( ... ) {
                log::error(c_fost_base_core,
                    "An unknown exception was caught -- abandoning thread");
                boost::mutex::scoped_lock lock(j->first->m_mutex);
#ifdef FOST_NO_STD_EXCEPTION_PTR
                j->first->m_exception = json("{Unknown exception}");
#else
                j->first->m_exception = std::current_exception();
#endif
                terminate = true; // Kill the thread after an unknown exception
            }
            {// Notify futures
                boost::mutex::scoped_lock lock(j->first->m_mutex);
                j->first->m_completed = true;
                j->first->m_has_result.notify_all();
            }
            if ( !terminate ) {
                boost::mutex::scoped_lock lock(self->m_mutex);
                terminate = self->m_terminate;
            }
        }
    } while ( !terminate );
}


/*
    fostlib::detail::future_result< void >
*/


fostlib::detail::future_result< void >::future_result()
: m_completed( false ) {
}


fostlib::detail::future_result< void >::~future_result() {
}


#ifdef FOST_NO_STD_EXCEPTION_PTR

fostlib::json fostlib::detail::future_result< void >::exception() {
    boost::mutex::scoped_lock lock( m_mutex );
    if ( !this->completed() )
        m_has_result.wait( lock, boost::lambda::var( m_completed ) );
    return m_exception;
}


void fostlib::detail::future_result<void>::wait() {
    json e(exception());
    if ( not e.isnull() )
        throw exceptions::forwarded(e);
}

#else

std::exception_ptr fostlib::detail::future_result< void >::exception() {
    boost::mutex::scoped_lock lock( m_mutex );
    if ( !this->completed() )
        m_has_result.wait( lock, boost::lambda::var( m_completed ) );
    return m_exception;
}


void fostlib::detail::future_result<void>::wait() {
    std::exception_ptr e(exception());
    if ( e )
        std::rethrow_exception(e);
}

#endif


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

