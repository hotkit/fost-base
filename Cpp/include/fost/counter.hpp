/*
    Copyright 1997-2018, Felspar Co Ltd. http://support.felspar.com/
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


    /// A single performance counter
    class FOST_CORE_DECLSPEC performance final {
        std::atomic<int64_t> count;
        const module * const parent;
        const jcursor path;
    public:
        /// Construct a performance counter
        performance(const module &, jcursor, int64_t value = 0);
        /// Construct a performance counter with extended path
        template<typename... Ss>
        performance(const module &m, const Ss &... ss)
        : performance(m, jcursor(ss...)) {
        }
        /// Make movable
        performance(performance &&p);
        /// Destruct the performance counter
        ~performance();

        /// Increase the performance count
        int64_t operator ++ () {
            return ++count;
        }
        /// Add to the performance count
        int64_t operator += (int64_t v) {
            return count += v;
        }
        /// Decrease the performance count
        int64_t operator -- () {
            return --count;
        }

        /// The current value
        int64_t value() const {
            return count.load();
        }
        /// The module relative path
        const jcursor &rel_path() const {
            return path;
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
