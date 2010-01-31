/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_WORKERPOOL_HPP
#define FOST_DETAIL_WORKERPOOL_HPP
#pragma once


#include <fost/thread.hpp>


namespace fostlib {


    namespace detail {


        template< typename R >
        R execute_future( boost::function< R ( void ) > lambda, boost::function< void ( void ) > completion ) {
            R r = lambda();
            completion();
            return r;
        }


    }


    class FOST_CORE_DECLSPEC workerpool : boost::noncopyable {
        struct implementation;
        implementation *impl;
        boost::shared_ptr< worker > assign();
        void replace(boost::shared_ptr< worker >);
        public:
            workerpool();
            ~workerpool();

            /*
                Execute any arbitrary nullary lambda that returns some value in any
                available worker from the pool.
            */
            template< typename R >
            future< R > f ( boost::function< R ( void ) > lambda ) {
                boost::shared_ptr< worker > w = assign();
                boost::function< void ( void ) > completion = boost::lambda::bind(
                    &workerpool::replace, this, w
                );
                boost::function< R ( void ) > future_lambda = boost::lambda::bind(
                    &detail::execute_future< R >, lambda, completion
                );
                return future< R >( w->run( future_lambda ) );
            }

            /*
                Obtain some metrics about the current state of the pool.
            */
            std::size_t available();
            std::size_t peak_used();
    };


}


#endif // FOST_DETAIL_WORKERPOOL_HPP
