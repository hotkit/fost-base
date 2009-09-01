/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
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


    class FOST_DATETIME_DECLSPEC date {
        boost::gregorian::date m_date;
        friend struct fostlib::coercer< boost::gregorian::date, date >;
    public:
        date();
        explicit date( boost::gregorian::date );
        date( int year, int month, int day );

        bool operator == ( const date &d ) const;
        bool operator != ( const date &d ) const;
    };


    template<>
    struct coercer< boost::gregorian::date, date > {
        boost::gregorian::date coerce( date d ) {
            return d.m_date;
        }
    };
    template<>
    struct FOST_DATETIME_DECLSPEC coercer< date, boost::gregorian::date > {
        date coerce( boost::gregorian::date );
    };

    template<>
    struct FOST_DATETIME_DECLSPEC coercer< string, date > {
        string coerce( date );
    };

    template<>
    struct coercer< json, date > {
        json coerce( date d ) {
            return json( fostlib::coerce< string >( d ) );
        }
    };
    template<>
    struct FOST_DATETIME_DECLSPEC coercer< date, json > {
        date coerce( const json & );
    };


}


namespace std {


    inline fostlib::ostream &operator << ( fostlib::ostream &o, fostlib::date d ) {
        return o << fostlib::coerce< fostlib::string >( d );
    }


}


#endif // FOST_DETAIL_DATE_HPP

