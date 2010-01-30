/*
    Copyright 1997-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COUNTER_HPP
#define FOST_COUNTER_HPP
#pragma once


#include <fost/config.hpp>


namespace fostlib {


    /// A thread safe counter
    class FOST_CORE_DECLSPEC counter : boost::noncopyable {
        struct counter_impl;
    public:
        counter();
        virtual ~counter();

        int operator ++();
        int value() const;

    private:
        counter_impl *m_impl;
    };


}


#endif // FOST_COUNTER_HPP

