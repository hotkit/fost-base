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
    class timer final {
        std::chrono::time_point<std::chrono::steady_clock> started;
    public:
        /// Construct and start the timer
        timer()
        : started(std::chrono::steady_clock::now()) {
        }

        /// The number of seconds that have elapsed
        template<typename Duration>
        Duration elapsed() const {
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<Duration>(now - started);
        }

        /// Return the number of seconds
        double seconds() const {
            auto now = std::chrono::steady_clock::now();
            auto taken = now - started;
            return double(taken.count()) * decltype(taken)::period::num / decltype(taken)::period::den;
        }

        /// Reset the timer so we can start to record again
        void reset() {
            started = std::chrono::steady_clock::now();
        }
    };


    template<typename Duration>
    class time_profile final {
        using sample_store = std::vector<std::pair<Duration, std::atomic<int64_t>>>;
        sample_store samples;
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
        /// Record based on the current timer value
        int64_t record(const timer &t) {
            return record(t.elapsed<Duration>());
        }

        /// Return the number of samples in the bucket
        int64_t operator [] (std::size_t b) const {
            return samples[b].second.load();
        }
        /// Return the number of overflowed samples
        int64_t overflow() const {
            return m_overflow.load();
        }

        /// Allow iteration
        using const_iterator = typename sample_store::const_iterator;
        const_iterator begin() const { return samples.begin(); }
        const_iterator end() const { return samples.end(); }

        /// The number of sample buckets
        std::size_t size() const {
            return samples.size();
        }

        /// The time for the last sample bucket
        Duration max_time() const {
            return samples.back().first;
        }
    };


    /// Convert the time profile to JSON.
    template<typename D>
    struct coercer<json, time_profile<D>> {
        /// Performs the coercions
        json coerce(const time_profile<D> &tp) {
            D last{};
            fostlib::json::array_t samples;
            for ( const auto &b : tp ) {
                const auto reading = b.second.load();
                if ( reading ) {
                    json::object_t sample;
                    sample["from"] = last.count();
                    sample["to"] = b.first.count();
                    sample["count"] = reading;
                    samples.push_back(sample);
                }
                last = b.first;
            }
            const auto of = tp.overflow();
            if ( of ) samples.push_back(json(of));
            return samples;
        }
    };


}


#endif // FOST_TIMER_HPP
