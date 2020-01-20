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

    /**
     * The code needs to grab the pointer value before calling the
     * constructor because the evaluation order of arguments is not
     * specified in C++, meaning that although the `ptr.get()` appears
     * in the source before the `std::move`, it may be evaluated
     * afterwards, leading to the wrong pointer value being captured.
     *
     * The following body would thus trigger undefined behaviour:
     * ```cpp
     * return boost::shared_ptr<T>(
     *      ptr.get(), [ptr = std::move(ptr)](T *) mutable { ptr.reset(); })
     *```
     */
    template<typename T>
    inline boost::shared_ptr<T> to_boost(std::shared_ptr<T> ptr) {
        T *const p = ptr.get();
        return boost::shared_ptr<T>(
                p, [ptr = std::move(ptr)](T *) mutable { ptr.reset(); });
    }

    template<typename T>
    inline std::shared_ptr<T> to_std(boost::shared_ptr<T> ptr) {
        T *const p = ptr.get();
        return std::shared_ptr<T>(
                p, [ptr = std::move(ptr)](T *) mutable { ptr.reset(); });
    }


}
