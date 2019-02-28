/**
    Copyright 2011-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
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
    /**
        The clock resolution for the start time may not be as high as one
        would want.
     */
    class timer final {
        std::chrono::steady_clock::time_point steady;
        std::chrono::system_clock::time_point start;

      public:
        /// Construct and start the timer
        timer()
        : steady{std::chrono::steady_clock::now()},
          start{std::chrono::system_clock::now()} {}

        /// Return when the timer started or was last reset
        auto started() const { return start; }

        /// The number of seconds that have elapsed
        template<typename Duration>
        Duration elapsed() const {
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<Duration>(now - steady);
        }

        /// Return the number of seconds
        double seconds() const {
            auto const now = std::chrono::steady_clock::now();
            auto const taken = now - steady;
            return double(taken.count()) * decltype(taken)::period::num
                    / decltype(taken)::period::den;
        }

        /// Reset the timer so we can start to record again
        void reset() {
            steady = std::chrono::steady_clock::now();
            start = std::chrono::system_clock::now();
        }
    };


    /// Record timings for some operation so that the distribution can be
    /// analysed later on. It is thread safe to use the `record` method from
    /// multiple threads at the same time.
    template<typename Duration>
    class time_profile final {
        using sample_store =
                std::vector<std::pair<Duration, std::atomic<int64_t>>>;
        sample_store samples;
        std::atomic<int64_t> m_overflow;
        std::atomic<typename Duration::rep> m_highwater;

      public:
        /// Create a time profile across the requested parematers. The smallest
        /// bucket will be mimimum duration long and the growth factor will be
        /// applied to scale the requested number of buckets. The first bucket
        /// always starts at zero time.
        time_profile(
                Duration width,
                double factor,
                std::size_t buckets,
                Duration offset = Duration{})
        : samples(buckets), m_overflow{}, m_highwater{} {
            auto last = offset;
            for (auto &b : samples) {
                last = b.first = last + width;
                width = Duration(
                        typename Duration::rep(width.count() * factor));
            }
        }

        /// Record a sample
        int64_t record(Duration tm) {
            auto into = std::upper_bound(
                    samples.begin(), samples.end(), tm,
                    [](auto &left, auto &right) { return left < right.first; });
            if (into == samples.end()) {
                while (true) {
                    auto high = m_highwater.load();
                    if (tm.count() > high) {
                        m_highwater.compare_exchange_strong(high, tm.count());
                    } else {
                        return ++m_overflow;
                    }
                }
            } else {
                return ++into->second;
            }
        }
        /// Record based on the current timer value
        int64_t record(const timer &t) { return record(t.elapsed<Duration>()); }

        /// Return the number of samples in the bucket
        int64_t operator[](std::size_t b) const {
            return samples[b].second.load();
        }
        /// Return the number of overflowed samples
        int64_t overflow() const { return m_overflow.load(); }
        /// Returns the largest sample
        Duration maximum() const { return Duration(m_highwater.load()); }

        /// Allow iteration
        using const_iterator = typename sample_store::const_iterator;
        const_iterator begin() const { return samples.begin(); }
        const_iterator end() const { return samples.end(); }

        /// The number of sample buckets
        std::size_t size() const { return samples.size(); }

        /// The time for the last sample bucket
        Duration max_time() const { return samples.back().first; }
    };


    /// Convert the time profile to JSON.
    template<typename D>
    struct coercer<json, time_profile<D>> {
        /// Performs the coercions
        json coerce(const time_profile<D> &tp) {
            D last{};
            fostlib::json::array_t samples;
            for (const auto &b : tp) {
                const auto reading = b.second.load();
                if (reading) {
                    json::object_t sample;
                    sample["from"] = last.count();
                    sample["to"] = b.first.count();
                    sample["count"] = reading;
                    samples.push_back(sample);
                }
                last = b.first;
            }
            const auto of = tp.overflow();
            if (of) {
                json::object_t sample;
                sample["from"] = last.count();
                sample["count"] = of;
                sample["max"] = tp.maximum().count();
                samples.push_back(sample);
            }
            return samples;
        }
    };


}


#endif // FOST_TIMER_HPP
