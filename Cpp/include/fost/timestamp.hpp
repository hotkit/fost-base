/**
    Copyright 2000-2019, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_TIMESTAMP_HPP
#define FOST_TIMESTAMP_HPP
#pragma once


#include <fost/date.hpp>
#include <fost/time.hpp>
#include <fost/timediff.hpp>
#include <fost/utility-nullable.hpp>
#include <fost/file.hpp>

#include <chrono>
#include <utility>


namespace fostlib {


    /// Stores a time and date together with a time zone. UTC is preferred.
    class FOST_CORE_DECLSPEC timestamp {
        boost::posix_time::ptime m_ts;
        friend class date;
        friend struct fostlib::coercer<boost::posix_time::ptime, timestamp>;

      public:
        /// Construct an indeterminate timestamp
        timestamp() {}
        /// Construct a timestamp from a Boost POSIX ptime
        explicit timestamp(boost::posix_time::ptime pt) : m_ts(pt) {}
        /// Construct a timestamp from a date (set the time to midnight)
        explicit timestamp(const date &d) : m_ts(d.m_date) {}
        /// Construct a timestamp for midnight at the start of the specified day
        timestamp(
                int year,
                int month,
                int day,
                int hour = 0,
                int minute = 0,
                int seconds = 0,
                int microseconds = 0)
        : m_ts(boost::gregorian::date(year, month, day),
               boost::posix_time::time_duration(
                       hour, minute, seconds, microseconds)) {}

        /// The zone info associated with this time stamp
        accessors<zoneinfo> timezone;

        /// The current time
        static timestamp now() {
            return timestamp(
                    boost::posix_time::microsec_clock::universal_time());
        }

        /// Compare time stamps for equality
        bool operator==(const timestamp &ts) const { return m_ts == ts.m_ts; }
        /// Compare time stamps for inequality
        bool operator!=(const timestamp &ts) const { return m_ts != ts.m_ts; }

        /// Compare time stamps for size
        bool operator<(const timestamp &ts) const { return m_ts < ts.m_ts; }
        /// Compare time stamps for size
        bool operator<=(const timestamp &ts) const { return m_ts <= ts.m_ts; }
        /// Compare time stamps for size
        bool operator>(const timestamp &ts) const { return m_ts > ts.m_ts; }
        /// Compare time stamps for size
        bool operator>=(const timestamp &ts) const { return m_ts >= ts.m_ts; }

        /// Allow change to the timestamp
        timestamp &operator+=(const timediff &td) {
            m_ts += td;
            return *this;
        }
        /// Allow us to add a timediff
        timestamp operator+(const timediff &td) const {
            return timestamp(m_ts + td);
        }

        /// Allow change to the timestamp
        timestamp &operator-=(const timediff &td) {
            m_ts -= td;
            return *this;
        }
        /// Allow us to subtract a timediff
        timestamp operator-(const timediff &td) const {
            return timestamp(m_ts - td);
        }

        /// Get a timediff between the two time stamps
        timediff operator-(const timestamp &ts) const { return m_ts - ts.m_ts; }
    };


    /// Construct a date from the time stamp
    inline date::date(const timestamp &ts) : m_date(ts.m_ts.date()) {}


    /// This date format is used in emails and HTTP
    struct FOST_CORE_DECLSPEC rfc1123_timestamp_tag {
        static void do_encode(fostlib::nliteral from, ascii_string &into);
        static void do_encode(const ascii_string &from, ascii_string &into);
        static void check_encoded(const ascii_string &s);
    };
    typedef tagged_string<rfc1123_timestamp_tag, ascii_string> rfc1123_timestamp;


    /// Allow a time stamp to be coerced to its date
    template<>
    inline date coerce<date, timestamp>(const timestamp &ts) {
        return date(ts);
    }


    /// Allow coercing of timestamps to and from common types
    template<>
    struct coercer<boost::posix_time::ptime, timestamp> {
        boost::posix_time::ptime coerce(timestamp t) { return t.m_ts; }
    };
    template<>
    struct coercer<timestamp, boost::posix_time::ptime> {
        timestamp coerce(boost::posix_time::ptime p) { return timestamp(p); }
    };

    /// Turn a timestamp into a (near) ISO formatted string for UTC
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, timestamp> {
        /// The default string format is ISO with the 'T' separator exchanged
        /// for a space
        string coerce(timestamp);
    };
    /// Turns an ISO formatted time stamp string into a timestamp
    template<>
    struct FOST_CORE_DECLSPEC coercer<timestamp, string> {
        /// The default string format is ISO with the 'T' separator exchanged
        /// for a space
        timestamp coerce(const string &);
    };

    /// Coerce a timestamp to the standard date format used in SMTP, HTTP etc.
    template<>
    struct FOST_CORE_DECLSPEC coercer<rfc1123_timestamp, timestamp> {
        rfc1123_timestamp coerce(timestamp);
    };

    /// Change the string type of a rfc1123 timestamp
    template<>
    struct coercer<string, rfc1123_timestamp> {
        string coerce(const rfc1123_timestamp &t) {
            return fostlib::coerce<string>(t.underlying());
        }
    };
    /// Parse a timestamp in the common HTTP/SMTP format to a timestamp
    template<>
    struct FOST_CORE_DECLSPEC coercer<timestamp, rfc1123_timestamp> {
        timestamp coerce(const rfc1123_timestamp &t);
    };

    /// We need JSON as well if we are to be able to use this for database fields
    template<>
    struct coercer<json, timestamp> {
        json coerce(timestamp t) { return json(fostlib::coerce<string>(t)); }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<timestamp, json> {
        timestamp coerce(const json &ts) {
            return fostlib::coerce<timestamp>(fostlib::coerce<string>(ts));
        }
    };

    /// Get a format suitable for use in file names
    template<>
    struct coercer<fostlib::fs::path, timestamp> {
        fostlib::fs::path coerce(const timestamp &ts) {
            string s(fostlib::coerce<string>(ts));
            return fostlib::coerce<fostlib::fs::path>(replace_all(s, ":"));
        }
    };


    /// Conversion of `std::chrono::system_clock::time_point` to useful formats
    template<>
    struct coercer<string, std::chrono::system_clock::time_point> {
        string coerce(std::chrono::system_clock::time_point);
    };
    template<>
    struct coercer<json, std::chrono::system_clock::time_point> {
        json coerce(std::chrono::system_clock::time_point t) {
            return json{fostlib::coerce<string>(t)};
        }
    };


}


namespace std {


    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, fostlib::timestamp t) {
        return o << fostlib::coerce<fostlib::string>(t);
    }


}


#endif // FOST_TIMESTAMP_HPP
