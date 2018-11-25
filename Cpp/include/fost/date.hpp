/*
    Copyright 2000-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_DATE_HPP
#define FOST_DETAIL_DATE_HPP
#pragma once


#include <fost/rel_ops>
#include <fost/string.hpp>
#include <fost/timediff.hpp>


namespace fostlib {


    /// Stores a time and date together with a time zone. UTC is preferred.
    class timestamp;


    /// A date in the Gregorian calandar
    class FOST_CORE_DECLSPEC date : public rel_ops<date> {
        boost::gregorian::date m_date;
        friend class timestamp;
        friend struct fostlib::coercer<boost::gregorian::date, date>;

      public:
        /// Construct an empty date
        date() {}
        /// Allow a date to be constructed from a Boost date
        explicit date(boost::gregorian::date d) : m_date(d) {}
        /// Pull the date part out of a timestamp (impl in timestamp.hpp)
        explicit date(const timestamp &);
        /// Construct a date from year, month and day values
        date(int year, int month, int day) : m_date(year, month, day) {}

        /// Today (UTC)
        static date today() {
            return date(boost::gregorian::day_clock::universal_day());
        }

        /// Compare dates for equality
        bool operator==(const date &d) const { return m_date == d.m_date; }

        /// Compare two dates
        bool operator<(const date &d) const { return m_date < d.m_date; }

        /// Add days to a date
        date operator+(const days &d) const { return date(m_date + d); }

        /// Subtract days to a date
        date operator-(const days &d) const { return date(m_date - d); }
    };


    /// Allow a date to be coerced to a Boost date
    template<>
    struct coercer<boost::gregorian::date, date> {
        /// Performs the coercions
        boost::gregorian::date coerce(date d) { return d.m_date; }
    };
    /// Allow a date to be coerced from a Boost date.
    template<>
    struct FOST_CORE_DECLSPEC coercer<date, boost::gregorian::date> {
        /// Performs the coercions
        date coerce(boost::gregorian::date d) { return date(d); }
    };

    /// Allow a date to be coerced to a string
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, date> {
        /// Performs the coercions
        string coerce(date);
    };

    /// Allow a date to be coerced to a JSON representation
    template<>
    struct coercer<json, date> {
        /// Performs the coercions
        json coerce(date d) { return json(fostlib::coerce<string>(d)); }
    };
    /// Allow a date to be coerced from a JSON representation
    template<>
    struct FOST_CORE_DECLSPEC coercer<date, json> {
        /// Performs the coercions
        date coerce(const json &);
    };


}


namespace std {


    /// Allow a date to be output to a stream
    inline fostlib::ostream &operator<<(fostlib::ostream &o, fostlib::date d) {
        return o << fostlib::coerce<fostlib::string>(d);
    }


}


#endif // FOST_DETAIL_DATE_HPP
