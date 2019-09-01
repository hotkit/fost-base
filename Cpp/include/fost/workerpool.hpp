/**
    Copyright 2009-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#ifndef FOST_DETAIL_WORKERPOOL_HPP
#define FOST_DETAIL_WORKERPOOL_HPP
#pragma once


#include <fost/thread.hpp>


namespace fostlib {


    /// An unbounded pool of workers
    class FOST_CORE_DECLSPEC workerpool : boost::noncopyable {
        struct implementation;
        implementation *impl;
        boost::shared_ptr<worker> assign();
        void replace(boost::shared_ptr<worker>);

      public:
        /// Construct an empty worker pool
        workerpool();
        /// Wait for all outstanding work to either terminate or complete
        ~workerpool();

        /// Execute any arbitrary nullary lambda that returns some
        /// value in any available worker from the pool.
        template<typename R>
        future<R> f(std::function<R(void)> lambda) {
            boost::shared_ptr<worker> w = assign();
            return future<R>(w->run<R>([this, w, lambda]() -> R {
                R r{lambda()};
                this->replace(w);
                return r;
            }));
        }

        /// The number of workers not working
        std::size_t available();
        /// The total number of workers
        std::size_t peak_used();
    };


}


#endif // FOST_DETAIL_WORKERPOOL_HPP
