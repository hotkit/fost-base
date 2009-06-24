/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DATE_HPP
#define FOST_DATE_HPP
#pragma once


#include <boost/date_time/gregorian/gregorian_types.hpp>


namespace fostlib {


    class FOST_DATETIME_DECLSPEC date {
        boost::gregorian::date m_date;
    public:
        date();

        bool operator == ( const date &d ) const;
        bool operator != ( const date &d ) const;
    };


}


#endif // FOST_DATE_HPP

