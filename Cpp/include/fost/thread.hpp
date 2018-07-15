/*
    Copyright 1997-2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_THREAD_HPP
#define FOST_THREAD_HPP
#pragma once


#include <fost/timediff.hpp>
#include <fost/threadsafe-store.hpp>

#include <condition_variable>
#include <functional>
#include <mutex>


namespace fostlib {


    namespace detail {
        /// The class that actually stores the future. Always pass to fostlib::future rather than use it directly
        template< typename R > class future_result;
    }

    /// Represents a single a thread which can execute functions and return a fostlib::future.
    class FOST_CORE_DECLSPEC worker : boost::noncopyable {
    private:
        struct context;
        std::shared_ptr<context> self;
    public:
        /// Start a worker ready to accept new jobs
        worker();
        /// Terminate the worker, waiting for the current job to complete
        ~worker() noexcept;

        std::shared_ptr<detail::future_result<void>> operator() (std::function<void(void)> f);

        template< typename R >
        std::shared_ptr<detail::future_result<R>> operator() (std::function<R(void)> f) {
            return run<R>(f);
        }

        template<typename R, typename F>
        std::shared_ptr<detail::future_result<R>> run(F f) {
            std::shared_ptr<detail::future_result<R>> future{new detail::future_result<R>};
            queue(future, typename detail::future_result<R>::function(future, f));
            return future;
        }

        void terminate();

        /// Return the current number of workers
        static int workers();

    private:
        void queue(
            std::shared_ptr<detail::future_result<void>> j,
            std::function<void(void)> f) const;
        friend class detail::future_result<void>;
    };

    namespace detail {
        template<>
        class FOST_CORE_DECLSPEC future_result< void > {
        protected:
            future_result();
        public:
/**
    This is used to turn off use of `std::exception_ptr` for platforms
    that don't have it.
 */
#ifdef FOST_NO_STD_EXCEPTION_PTR
            using exception_type = json;
#else
            using exception_type = std::exception_ptr;
#endif
            virtual ~future_result();

            /// Blocks waiting for the result to become available
            void wait();
            /// Blocks for up to the specified time period waiting for the result
            void wait(const timediff &);
            /// Blocks waiting to see if there is an exception or not
            exception_type exception();
            /// Returns true if the result is available
            bool completed() const {
                return m_completed;
            }

        private:
            bool m_completed;
            exception_type m_exception;

            std::mutex m_mutex;
            std::condition_variable m_has_result;

            friend class fostlib::worker;
        };

        template< typename R >
        class future_result : public future_result< void > {
        private:
            struct function {
                function(std::shared_ptr<future_result<R>> j, std::function<R(void)> f)
                : m_future(j), m_f(f) {
                }
                void operator() () {
                    m_future->m_result = m_f();
                }
            private:
                std::shared_ptr<future_result<R>> m_future;
                std::function<R(void)> m_f;
            };

            future_result()
            : m_result() {
            }
        public:
            /// Return the value contained in the future
            const R &result() {
                wait();
                return m_result;
            }
        private:
            R m_result;
            friend class fostlib::worker;
        };
    }


    /// Specialisation to allow for void return types on a worker
    template <> inline
    std::shared_ptr<detail::future_result<void>> worker::run(std::function<void(void)> f) {
        std::shared_ptr<detail::future_result<void>> future{new detail::future_result<void>};
        queue(future, f);
        return future;
    }


    template< typename O > class in_process;

    /// Represents the result of a calculation that will finish in the future
    template< typename R >
    class future {
    public:
        /// Construct an empty future
        future() {}
        /// Construct a future that will get a result
        future(std::shared_ptr<detail::future_result<R>> r)
        : m_result(r) {
        }

        /// Blocks waiting for the future to become available
        const R &operator() () const {
            assert_valid();
            return m_result->result();
        }
        /// Blocks waiting to see if the future will result in an exception
        const nullable<string> &exception() const {
            assert_valid();
            return m_result->exception();
        }
        /// Returns true if the result is available
        bool available() const {
            assert_valid();
            return m_result->completed();
        }
        /// Returns true if the result is available within the specified timediff
        bool available(const timediff &td) {
            assert_valid();
            m_result->wait(td);
            return m_result->completed();
        }

        /// Allow us to compare two futures
        bool operator == (const future &f) const {
            return m_result == f.m_result;
        }

    private:
        void assert_valid() const {
            if ( ! m_result.get() )
                throw exceptions::null("The result/future has not been initialised");
        }
        std::shared_ptr<detail::future_result<R>> m_result;

        template< typename O > friend class in_process;
    };
    /// A future where we don't care about the return value
    template <>
    class future<void> {
        std::shared_ptr<detail::future_result<void>> m_result;
        void assert_valid() const {
            if ( ! m_result.get() )
                throw exceptions::null("The result/future has not been initialised");
        }
    public:
        /// Construct an empty future
        future() {}
        /// Construct a future that will get a result
        future(std::shared_ptr<detail::future_result<void>> r)
        : m_result(r) {
        }

        /// Block waiting for the future to become available
        void operator () () {
            assert_valid();
            m_result->wait();
        }

        /// Allow us to compare two futures
        bool operator == ( const future &f ) const {
            return m_result == f.m_result;
        }
    };


    /// Wraps fostlib::worker to give a simpler synchronous and asynchronous processing model
    template< typename O >
    class in_process : private worker {
    private:
        template< typename B >
        struct functor {
            functor( O &o, std::function<B(O &)> b ) : m_o( o ), m_f( b ) {}
            B operator() () { return m_f( m_o ); }
            O &m_o; std::function<B(O &)> m_f;
        };
    public:
        /// Construct the in_process from anything convertible to a function returning O*
        template< typename F >
        in_process( F c )
        : object( (worker::operator() ( std::function<O*(void)>(c) ))->result() ) {
        }
        explicit in_process( O *o )
        : object( o ) {
        }

        template< typename B >
        B synchronous( std::function<B(O &)> b ) {
            return asynchronous<B>(b)();
        }

        template< typename B >
        future< B > asynchronous(std::function<B(O &)> b) {
            return future< B >( worker::operator ()<B>( functor< B >( *object, b ) ) );
        }

    private:
        std::unique_ptr< O > object;
    };


}


#endif // FOST_THREAD_HPP

