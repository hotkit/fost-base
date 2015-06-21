/*
    Copyright 1997-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_COUNTER_HPP
#define FOST_COUNTER_HPP
#pragma once


#include <fost/log.hpp>
#include <atomic>


namespace fostlib {


    /// A thread safe counter
    class FOST_CORE_DECLSPEC
    [[deprecated("Use the new C++11 atomics rather than this")]]
            counter : boost::noncopyable {
        struct counter_impl;
    public:
        counter();
        virtual ~counter();

        int operator ++();
        int operator --();
        int value() const;

    private:
        counter_impl *m_impl;
    };


    /// A single performance counter
    class FOST_CORE_DECLSPEC performance {
        const jcursor path;
        std::atomic<int64_t> count;
    public:
        /// Construct a performance counter
        performance(const module::data &module,
            const string &section, const string &name);
        /// Destruct the performance counter
        ~performance();

        /// Increase the performance count
        int64_t operator ++ () {
            return ++count;
        }
        /// Decrease the performance count
        int64_t operator -- () {
            return --count;
        }

        /// The current value
        int64_t value() const {
            return count.load();
        }

        /// Return a JSON structure that describes all of the current
        /// counter values
        static json current();
    };


    namespace log {
        /// Constant containing the performance log level
        const unsigned g_performance_level = 0x700u;
        /// Define a new logger level 'performance'
        FSL_DEFINE_LOGGING_LEVEL(perf, g_performance_level);
    }


}


#endif // FOST_COUNTER_HPP
