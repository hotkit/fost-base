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


/// Allow inequality for differing types
template <typename T, typename Y>
inline bool operator != (const T& x, const Y& y) {
    return !(x==y);
}
/// Allow ordering for differing types
template <typename T, typename Y>
inline bool operator >  (const T& x, const Y& y) {
    return y<x;
}
/// Allow ordering for differing types
template <typename T, typename Y>
inline bool operator <= (const T& x, const Y& y) {
    return !(y<x);
}
/// Allow ordering for differing types
template <typename T, typename Y>
inline bool operator >= (const T& x, const Y& y) {
    return !(x<y);
}


#endif // FOST_RELOPS_HPP
