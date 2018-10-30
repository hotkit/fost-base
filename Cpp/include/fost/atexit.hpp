/**
    Copyright 2009-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_ATEXIT_HPP
#define FOST_ATEXIT_HPP
#pragma once


#include <functional>


namespace fostlib {


    /// Register a function to be run before the program completes. Functions
    /// are run in an uspecified order.
    FOST_CORE_DECLSPEC void atexit(std::function<void(void)> function);


}


#endif // FOST_ATEXIT_HPP
