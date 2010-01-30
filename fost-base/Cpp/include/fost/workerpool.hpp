/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_WORKERPOOL_HPP
#define FOST_DETAIL_WORKERPOOL_HPP
#pragma once


#include <fost/thread.hpp>


namespace fostlib {


    /// A queue of similar tasks
    template< typename R > class workerpool;


    /// Specialisation of the workqueue for the case where there is no return value
    template<>
    class FOST_CORE_DECLSPEC workerpool< void > {
        typedef detail::future_result< void > future_result_type;
        public:
            /// The type of work done by this queue
            typedef boost::function0< void > function_type;

            /// Want to allow sub-classes to destruct cleanly
            virtual ~workerpool();

            /// Queue a function for execution
            void operator () ( function_type f );
            /// Wait for the next function to complete
            void operator () ();
    };


    /// Specialisation for work queues when there is a return type
    template< typename R >
    class workerpool : public workerpool< void > {
        typedef R return_type;
        typedef boost::function0< R > function_type;
        public:
            return_type operator () () {
                throw exceptions::not_implemented("workerpool<R>::operator () ()");
            }
            void operator () ( function_type f ) {
                throw exceptions::not_implemented("workerpool<R>::operator () (function_type)");
            }
    };


}


#endif // FOST_DETAIL_WORKERPOOL_HPP
