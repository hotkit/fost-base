/**
    Copyright 2020 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <boost/shared_ptr.hpp>
#include <memory>


namespace fostlib {


    //// ## Conversion between `std` and `boost` `shared_ptr`s

    template<typename T>
    inline boost::shared_ptr<T> to_boost(std::shared_ptr<T> ptr) {
        return boost::shared_ptr<T>(
                ptr.get(),
                [ptr = std::move(ptr)](T *) mutable { ptr.reset(); });
    }

    template<typename T>
    inline std::shared_ptr<T> to_std(boost::shared_ptr<T> ptr) {
        return std::shared_ptr<T>(
                ptr.get(),
                [ptr = std::move(ptr)](T *) mutable { ptr.reset(); });
    }


}
