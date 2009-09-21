/*
    Copyright 2000-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-datetime.hpp"
#include <fost/detail/date.hpp>

#include <boost/date_time/gregorian/gregorian.hpp>


using namespace fostlib;


fostlib::date::date() {
}
fostlib::date::date( boost::gregorian::date d )
: m_date( d ) {
}
fostlib::date::date( int year, int month, int day )
: m_date( year, month, day ) {
}


bool fostlib::date::operator == ( const date &r ) const {
    return m_date == r.m_date;
}
bool fostlib::date::operator != ( const date &r ) const {
    return m_date != r.m_date;
}


string fostlib::coercer< string, date >::coerce( date d ) {
    return fostlib::coerce< string >( boost::gregorian::to_iso_extended_string(fostlib::coerce< boost::gregorian::date >( d )) );
}

date fostlib::coercer< date, json >::coerce( const json &j ) {
    nullable< string > s( j.get< string >() );
    if ( s.isnull() )
        throw exceptions::not_implemented("fostlib::coercer< date, json >::coerce( const json &j ) -- with non-string JSON");
    return date( boost::gregorian::from_string( fostlib::coerce< std::string >( s.value() ) ) );
}
