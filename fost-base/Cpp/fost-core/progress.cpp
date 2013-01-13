/*
    Copyright 2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/progress.hpp>


using namespace fostlib;


fostlib::progress::progress(std::size_t upto)
: now(), last(upto) {
}


std::size_t fostlib::progress::operator ++ () {
    return ++now;
}


std::size_t fostlib::progress::operator += (std::size_t amount) {
    return now += amount;
}

