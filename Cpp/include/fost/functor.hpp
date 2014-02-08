/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_FUNCTOR_HPP
#define FOST_FUNCTOR_HPP
#pragma once


#include <fost/config.hpp>


namespace fostlib {


    namespace functor {


        /// Identiry functor
        template< typename V >
        struct identity {
            /// The result type
            typedef V result_type;
            /// The argument type
            typedef V arg1_type;
            /// The identity function
            const result_type &operator () ( const result_type &v ) const {
                return v;
            }
        };


        /// A functor which holds a single value
        template< typename V >
        struct const_value {
            /// The value type
            typedef V result_type;
            /// Construct the functor with a provided value
            const_value( const result_type &v )
            : value( v ) {
            }
            /// Stores the value
            accessors< result_type > value;
            /// Call the functor
            const result_type &operator () () const {
                return value();
            }
        };


        /// A functor that calls coerce -- (TODO maybe to be generalised)
        template< typename T, typename F >
        struct coercer {
            /// The result type
            typedef T result_type;
            /// The argument type
            typedef F arg1_type;
            /// The coercion function
            result_type operator () ( const F &f ) const {
                return fostlib::coerce< T >( f );
            }
        };


        /// A unary functor that calls another unary functor
        template< typename R, typename A1 >
        struct apply {
            /// The result type
            typedef R result_type;
            /// The argument type
            typedef A1 arg1_type;
            /// The functor itself
            template< typename F >
            result_type operator () ( F f, const arg1_type &a1 ) const {
                return f(a1);
            }
        };


        /// A unary functor that calls another nullary functor dropping the argument
        template< typename R, typename A1 >
        struct apply_without_arguments {
            /// The result type
            typedef R result_type;
            /// The argument type
            typedef A1 arg1_type;
            /// The functor itself
            template< typename F >
            result_type operator () ( F f, const arg1_type & ) const {
                return f();
            }
        };


    }


}


#endif // FOST_FUNCTOR_HPP

