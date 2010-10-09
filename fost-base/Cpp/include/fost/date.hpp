/*
    Copyright 2000-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_DATE_HPP
#define FOST_DETAIL_DATE_HPP
#pragma once


#include <fost/core>
#include <boost/date_time/gregorian/gregorian_types.hpp>


namespace fostlib {


    /// A date in the Gregorian calandar
    class FOST_CORE_DECLSPEC date {
        boost::gregorian::date m_date;
        friend struct fostlib::coercer< boost::gregorian::date, date >;
    public:
        /// Construct an empty date
        date();
        /// Allow a date to be constructed from a Boost date
        explicit date( boost::gregorian::date );
        /// Construct a date from year, month and day values
        date( int year, int month, int day );

        /// Compare dates for equality
        bool operator == ( const date &d ) const;
        /// Compare dates for inequality
        bool operator != ( const date &d ) const;
    };


    /// Allow a date to be coerced to a Boost date
    template<>
    struct coercer< boost::gregorian::date, date > {
        /// Performs the coercions
        boost::gregorian::date coerce( date d ) {
            return d.m_date;
        }
    };
    /// Allow a date to be coerced from a Boost date.
    template<>
    struct FOST_CORE_DECLSPEC coercer< date, boost::gregorian::date > {
        /// Performs the coercions
        date coerce( boost::gregorian::date d ) {
            return date( d );
        }
    };

    /// Allow a date to be coerced to a string
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, date > {
        /// Performs the coercions
        string coerce( date );
    };

    /// Allow a date to be coerced to a JSON representation
    template<>
    struct coercer< json, date > {
        /// Performs the coercions
        json coerce( date d ) {
            return json( fostlib::coerce< string >( d ) );
        }
    };
    /// Allow a date to be coerced from a JSON representation
    template<>
    struct FOST_CORE_DECLSPEC coercer< date, json > {
        /// Performs the coercions
        date coerce( const json & );
    };


}


namespace std {


    /// Allow a date to be output to a stream
    inline fostlib::ostream &operator << ( fostlib::ostream &o, fostlib::date d ) {
        return o << fostlib::coerce< fostlib::string >( d );
    }


}


#endif // FOST_DETAIL_DATE_HPP
