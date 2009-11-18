/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_MEMOIZE_HPP
#define FOST_DETAIL_MEMOIZE_HPP
#pragma once


#include <fost/config.hpp>
#include <boost/variant.hpp>


namespace fostlib {


    /// Abstracts the logic for handling the memoization of expensive functions.
    template< typename R >
    class memoize {
        public:
            /// The type of the value that is memoized
            typedef R value_type;
            /// The type of the lambda that is required.
            typedef boost::function< value_type ( void ) > lambda_type;

            /// Construct the memoization wrapper with a nullary lambda that performs an expensive computation
            memoize( lambda_type f )
            : m_memory( f ) {
            }

            /// Returns the value of the function caching the result
            value_type operator () ();

        private:
            typedef boost::variant< value_type, lambda_type > memory_type;
            memory_type m_memory;

            struct fetch : public boost::static_visitor< value_type > {
                value_type operator () ( value_type r ) const {
                    return r;
                }
                value_type operator () ( lambda_type f ) const {
                    return f();
                }
            };
    };


}


template< typename R >
inline R fostlib::memoize< R >::operator () () {
    value_type r = boost::apply_visitor( fetch(), m_memory );
    m_memory = r;
    return r;
}


#endif // FOST_DETAIL_MEMOIZE_HPP
