/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-datetime.hpp"
#include <fost/detail/date.hpp>


using namespace fostlib;


fostlib::date::date() {
}


bool fostlib::date::operator == ( const date &r ) const {
    return m_date == r.m_date;
}
bool fostlib::date::operator != ( const date &r ) const {
    return m_date != r.m_date;
}
