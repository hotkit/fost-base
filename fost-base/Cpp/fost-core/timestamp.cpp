/*
    Copyright 2000-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/timestamp.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace fostlib;


fostlib::timestamp::timestamp() {
}
fostlib::timestamp::timestamp(boost::posix_time::ptime pt)
: m_ts(pt) {
}
fostlib::timestamp::timestamp( int year, int month, int day, int hour, int minute )
: m_ts( boost::gregorian::date(year, month, day),
       boost::posix_time::time_duration(hour, minute, 0) ) {
}

timestamp fostlib::timestamp::now() {
    return timestamp(boost::posix_time::microsec_clock::universal_time());
}


string fostlib::coercer< string, timestamp >::coerce( timestamp t ) {
    std::string s = boost::posix_time::to_iso_extended_string(
        fostlib::coerce< boost::posix_time::ptime >(t)) + "Z";
    return fostlib::coerce< string >(fostlib::coerce< ascii_string >(s));
}
rfc1123_timestamp fostlib::coercer<
    rfc1123_timestamp, timestamp
>::coerce( timestamp t ) {
    boost::posix_time::ptime ts = fostlib::coerce< boost::posix_time::ptime >(t);
    std::stringstream ss;
    ss.imbue(std::locale(
        ss.getloc(),
        new boost::posix_time::time_facet("%a, %d %b %Y %H:%M:%S +0000")
    ));
    ss << ts;
    return rfc1123_timestamp(ascii_string(ss.str()));
}

timestamp fostlib::coercer< timestamp, json >::coerce( const json &j ) {
    nullable< string > s( j.get< string >() );
    if ( s.isnull() )
        throw exceptions::not_implemented(
            L"fostlib::coercer< timestamp, json >::coerce( const json &j ) -- "
            L"where the JSON is not a string");
    std::string repr = fostlib::coerce< ascii_string >( s.value() ).underlying().substr(0, 19);
    repr[10] = ' ';
    return timestamp( boost::posix_time::time_from_string(repr) );
}

