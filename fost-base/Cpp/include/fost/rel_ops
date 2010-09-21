/*
    Copyright 2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_RELOPS_HPP
#define FOST_RELOPS_HPP
#pragma once


#include <utility>


using namespace std::rel_ops;


namespace fostlib {


    /// Allows for comparison operators to be written in terms of the base operators.
    template< typename R >
    class rel_ops {
    public:
        /// Allow inequality for differing types
        template <typename Y>
        bool operator != (const Y& y) const {
            return !(self()==y);
        }
        /// Allow ordering for differing types
        template <typename Y>
        bool operator > (const Y& y) const {
            return y<self();
        }
        /// Allow ordering for differing types
        template <typename Y>
        bool operator <= (const Y& y) const {
            return !(y<self());
        }
        /// Allow ordering for differing types
        template <typename Y>
        bool operator >= (const Y& y) const {
            return !(self()<y);
        }

    private:
        const R &self() const {
            return *reinterpret_cast< const R * >(this);
        }
    };


}


#endif // FOST_RELOPS_HPP
