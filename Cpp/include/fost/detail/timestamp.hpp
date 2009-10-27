/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_TIMESTAMP_HPP
#define FOST_DETAIL_TIMESTAMP_HPP
#pragma once


#include <fost/string>
#include <fost/detail/date.hpp>
#include <fost/detail/time.hpp>

#include <boost/date_time/posix_time/posix_time_types.hpp>


namespace fostlib {


    /// Stores a time and date together with a time zone. UTC is preferred.
    class FOST_DATETIME_DECLSPEC timestamp {
        boost::posix_time::ptime m_ts;
        friend struct fostlib::coercer< boost::posix_time::ptime, timestamp >;
    public:
        timestamp();
        timestamp(boost::posix_time::ptime pt);

        accessors< zoneinfo > timezone;

        static timestamp now();

        bool operator == ( const timestamp &ts ) const;
        bool operator != ( const timestamp &ts ) const;
    };


    /// This date format is used in emails and HTTP where it is the date format
    struct FOST_DATETIME_DECLSPEC rfc1123_timestamp_tag {
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

    template<>
    struct FOST_DATETIME_DECLSPEC coercer< string, timestamp > {
        // The default string format is ISO with the 'T' separator exchanged for a space
        string coerce( timestamp );
    };
    template<>
    struct FOST_DATETIME_DECLSPEC coercer< rfc1123_timestamp, timestamp > {
        rfc1123_timestamp coerce( timestamp );
    };
    template<>
    struct FOST_DATETIME_DECLSPEC coercer< string, rfc1123_timestamp > {
        string coerce( const rfc1123_timestamp &t ) {
            return fostlib::coerce< string >( t.underlying() );
        }
    };

    /// We need JSON as well if we are to be able to use this for database fields
    template<>
    struct coercer< json, timestamp > {
        json coerce( timestamp t ) {
            return json( fostlib::coerce< string >( t ) );
        }
    };
    template<>
    struct FOST_DATETIME_DECLSPEC coercer< timestamp, json > {
        timestamp coerce( const json & );
    };


}


namespace std {


    inline fostlib::ostream &operator << ( fostlib::ostream &o, fostlib::timestamp t ) {
        return o << fostlib::coerce< fostlib::string >( t );
    }


}


#endif // FOST_DETAIL_TIMESTAMP_HPP
