/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DETAIL_TIMESTAMP_HPP
#define FOST_DETAIL_TIMESTAMP_HPP
#pragma once


#include <fost/core>
#include <fost/detail/date.hpp>
#include <fost/detail/time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>


namespace fostlib {


    class FOST_DATETIME_DECLSPEC timestamp {
        boost::posix_time::ptime m_ts;
        friend struct fostlib::coercer< boost::posix_time::ptime, timestamp >;
    public:
        timestamp();
        timestamp(boost::posix_time::ptime pt);

        static timestamp now();

        bool operator == ( const timestamp &ts ) const;
        bool operator != ( const timestamp &ts ) const;
    };


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
        string coerce( timestamp );
    };


}


#endif // FOST_DETAIL_TIMESTAMP_HPP
