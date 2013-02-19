/*
    Copyright 2000-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TIMESTAMP_HPP
#define FOST_TIMESTAMP_HPP
#pragma once


#include <fost/date.hpp>
#include <fost/time.hpp>
#include <fost/timediff.hpp>

#include <utility>
using namespace std::rel_ops;


namespace fostlib {


    /// Stores a time and date together with a time zone. UTC is preferred.
    class FOST_CORE_DECLSPEC timestamp {
        boost::posix_time::ptime m_ts;
        friend struct fostlib::coercer< boost::posix_time::ptime, timestamp >;
    public:
        /// Construct an indeterminate timestamp
        timestamp();
        /// Construct a timestamp from a Boost POSIX ptime
        explicit timestamp(boost::posix_time::ptime pt);
        /// Construct a timestamp for midnight at the start of the specified day
        timestamp( int year, int month, int day, int hour = 0, int minute = 0, int seconds = 0);

        /// The zone info associated with this time stamp
        accessors< zoneinfo > timezone;

        /// The current time
        static timestamp now();

        /// Compare time stamps for equality
        bool operator == ( const timestamp &ts ) const {
            return m_ts == ts.m_ts;
        }
        /// Compare time stamps for inequality
        bool operator != ( const timestamp &ts ) const {
            return m_ts != ts.m_ts;
        }

        /// Compare time stamps for size
        bool operator < ( const timestamp &ts ) const {
            return m_ts < ts.m_ts;
        }
        /// Compare time stamps for size
        bool operator > ( const timestamp &ts ) const {
            return m_ts > ts.m_ts;
        }

        /// Allow change to the timestamp
        timestamp &operator += ( const timediff &td ) {
            m_ts += td;
            return *this;
        }
        /// Allow us to add a timediff
        timestamp operator + ( const timediff &td ) {
            return timestamp(m_ts + td);
        }

        /// Allow change to the timestamp
        timestamp &operator -= ( const timediff &td ) {
            m_ts -= td;
            return *this;
        }
        /// Allow us to subtract a timediff
        timestamp operator - ( const timediff &td ) {
            return timestamp(m_ts - td);
        }
    };


    /// This date format is used in emails and HTTP
    struct FOST_CORE_DECLSPEC rfc1123_timestamp_tag {
        static void do_encode( fostlib::nliteral from, ascii_string &into );
        static void do_encode( const ascii_string &from, ascii_string &into );
        static void check_encoded( const ascii_string &s );
    };
    typedef tagged_string< rfc1123_timestamp_tag, ascii_string > rfc1123_timestamp;


    /// Allow coercing of timestamps to and from common types
    template<>
    struct coercer< boost::posix_time::ptime, timestamp > {
        boost::posix_time::ptime coerce( timestamp t ) {
            return t.m_ts;
        }
    };
    template<>
    struct coercer< timestamp, boost::posix_time::ptime > {
        timestamp coerce(boost::posix_time::ptime p) {
            return timestamp(p);
        }
    };

    /// Turn a timestamp into a (near) ISO formatted string for UTC
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, timestamp > {
        /// The default string format is ISO with the 'T' separator exchanged for a space
        string coerce( timestamp );
    };
    /// Coerce a timestamp to the standard date format used in SMTP, HTTP etc.
    template<>
    struct FOST_CORE_DECLSPEC coercer<
        rfc1123_timestamp, timestamp
    > {
        rfc1123_timestamp coerce( timestamp );
    };

    /// Change the string type of a rfc1123 timestamp
    template<>
    struct coercer< string, rfc1123_timestamp > {
        string coerce( const rfc1123_timestamp &t ) {
            return fostlib::coerce< string >( t.underlying() );
        }
    };
    /// Parse a timestamp in the common HTTP/SMTP format to a timestamp
    template<>
    struct FOST_CORE_DECLSPEC coercer<
        timestamp, rfc1123_timestamp
    > {
        timestamp coerce( const rfc1123_timestamp &t );
    };

    /// We need JSON as well if we are to be able to use this for database fields
    template<>
    struct coercer< json, timestamp > {
        json coerce( timestamp t ) {
            return json( fostlib::coerce< string >( t ) );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< timestamp, json > {
        timestamp coerce( const json & );
    };


}


inline
fostlib::timestamp operator + ( const fostlib::timestamp &ts, const fostlib::timediff &td ) {
    return fostlib::timestamp(ts) += td;
}


inline
fostlib::timestamp operator - ( const fostlib::timestamp &ts, const fostlib::timediff &td ) {
    return fostlib::timestamp(ts) -= td;
}


namespace std {


    inline fostlib::ostream &operator << ( fostlib::ostream &o, fostlib::timestamp t ) {
        return o << fostlib::coerce< fostlib::string >( t );
    }


}


#endif // FOST_TIMESTAMP_HPP
