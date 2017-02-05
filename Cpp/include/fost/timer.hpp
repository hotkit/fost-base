/*
    Copyright 2011-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TIMER_HPP
#define FOST_TIMER_HPP
#pragma once


#include <fost/core>

#include <atomic>
#include <algorithm>
#include <chrono>
#include <cmath>


namespace fostlib {


    /// A simple timer for measuring the duration of something
    class FOST_DATETIME_DECLSPEC timer final : boost::noncopyable {
        const std::chrono::time_point<std::chrono::steady_clock> started;
    public:
        /// Construct and start the timer
        timer()
        : started(std::chrono::steady_clock::now()) {
        }

        /// The number of seconds that have elapsed
        template<typename Duration = double>
        Duration elapsed() const {
            auto now = std::chrono::steady_clock::now();
            std::chrono::duration<Duration> taken = now - started;
            return taken.count();
        }
    };


    template<typename Duration>
    class time_profile final {
        std::vector<std::pair<Duration, std::atomic<int64_t>>> samples;
        std::atomic<int64_t> m_overflow;
   public:
        /// Create a time profile across the requested parematers. The smallest
        /// bucket will be mimimum duration long and the growth factor will be
        /// applied to scale the requested number of buckets. The first bucket
        /// always starts at zero time.
        time_profile(Duration width, double factor, std::size_t buckets)
        : samples(buckets), m_overflow{} {
            for ( auto &b : samples ) {
                b.first = width;
                width += Duration(uint64_t(width.count() * factor));
            }
        }

        /// Record a sample
        int64_t record(Duration tm) {
            auto into = std::upper_bound(samples.begin(), samples.end(), tm,
                [](auto &left, auto &right) {
                    return left < right.first;
                });
            if ( into == samples.end() ) {
                return ++m_overflow;
            } else {
                return ++into->second;
            }
        }

        /// Return the number of samples in the bucket
        int64_t operator [] (std::size_t b) const {
            return samples[b].second.load();
        }
        /// Return the number of overflowed samples
        int64_t overflow() const {
            return m_overflow.load();
        }

        /// The number of sample buckets
        std::size_t size() const {
            return samples.size();
        }

        /// The time for the last sample bucket
        Duration max_time() const {
            return samples.back().first;
        }
    };


}


#endif // FOST_TIMER_HPP
