/*
    Copyright 2000-2012, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/timestamp.hpp>
#include <fost/parse/parse.hpp>


using namespace fostlib;


/*
    fostlib::rfc1123_timestamp
*/


void fostlib::rfc1123_timestamp_tag::do_encode(
    fostlib::nliteral from, ascii_string &into
) {
    throw exceptions::not_implemented("fostlib::rfc1123_timestamp_tag::do_encode( fostlib::nliteral from, ascii_string &into )");
}


void fostlib::rfc1123_timestamp_tag::do_encode(
    const ascii_string &from, ascii_string &into
) {
    throw exceptions::not_implemented("fostlib::rfc1123_timestamp_tag::do_encode( const ascii_string &from, ascii_string &into )");
}


void fostlib::rfc1123_timestamp_tag::check_encoded(
    const ascii_string &s
) {
    try {
        if ( s.underlying().length() != 31 )
            throw exceptions::out_of_range< int >("RFC 1123 date format must be 31 characters", 31, 31, s.underlying().length());
    } catch ( exceptions::exception &e ) {
        e.info() << "String is " << s << std::endl;
        throw;
    }
}


rfc1123_timestamp fostlib::coercer<
    rfc1123_timestamp, timestamp
>::coerce( timestamp t ) {
    boost::posix_time::ptime ts =
        fostlib::coerce< boost::posix_time::ptime >(t);
    std::stringstream ss;
    ss.imbue(std::locale(
        ss.getloc(),
        new boost::posix_time::time_facet(
            "%a, %d %b %Y %H:%M:%S +0000")));
    ss << ts;
    return rfc1123_timestamp(ascii_string(ss.str()));
}


timestamp fostlib::coercer< timestamp, rfc1123_timestamp >::coerce(
    const rfc1123_timestamp &ts
) {
    int year, month, day, hour, minute, seconds;
    parser_lock lock;
    if ( !parse(lock, ts.underlying().underlying().c_str(), (
                (
                    (
                        boost::spirit::strlit< nliteral >( "Mon," )
                        | boost::spirit::strlit< nliteral >( "Tue," )
                        | boost::spirit::strlit< nliteral >( "Wed," )
                        | boost::spirit::strlit< nliteral >( "Thu," )
                        | boost::spirit::strlit< nliteral >( "Fri," )
                        | boost::spirit::strlit< nliteral >( "Sat," )
                        | boost::spirit::strlit< nliteral >( "Sun," )
                    )
                    >> *boost::spirit::space_p
                )
                >> boost::spirit::int_parser< int >()
                    [ phoenix::var(day) = phoenix::arg1 ]
                >> *boost::spirit::space_p
                >> (
                        boost::spirit::strlit< nliteral >( "Jan" )
                            [ phoenix::var(month) = 1 ]
                        | boost::spirit::strlit< nliteral >( "Feb" )
                            [ phoenix::var(month) = 2 ]
                        | boost::spirit::strlit< nliteral >( "Mar" )
                            [ phoenix::var(month) = 3 ]
                        | boost::spirit::strlit< nliteral >( "Apr" )
                            [ phoenix::var(month) = 4 ]
                        | boost::spirit::strlit< nliteral >( "May" )
                            [ phoenix::var(month) = 5 ]
                        | boost::spirit::strlit< nliteral >( "Jun" )
                            [ phoenix::var(month) = 6 ]
                        | boost::spirit::strlit< nliteral >( "Jul" )
                            [ phoenix::var(month) = 7 ]
                        | boost::spirit::strlit< nliteral >( "Aug" )
                            [ phoenix::var(month) = 8 ]
                        | boost::spirit::strlit< nliteral >( "Sep" )
                            [ phoenix::var(month) = 9 ]
                        | boost::spirit::strlit< nliteral >( "Oct" )
                            [ phoenix::var(month) = 10 ]
                        | boost::spirit::strlit< nliteral >( "Nov" )
                            [ phoenix::var(month) = 11 ]
                        | boost::spirit::strlit< nliteral >( "Dec" )
                            [ phoenix::var(month) = 12 ]
                )
                >> *boost::spirit::space_p
                >> boost::spirit::int_parser< int >()
                    [ phoenix::var(year) = phoenix::arg1 ]
                >> *boost::spirit::space_p
                >> boost::spirit::int_parser< int >()
                    [ phoenix::var(hour) = phoenix::arg1 ]
                >> boost::spirit::chlit< char >(':')
                >> boost::spirit::int_parser< int >()
                    [ phoenix::var(minute) = phoenix::arg1 ]
                >> boost::spirit::chlit< char >(':')
                >> boost::spirit::int_parser< int >()
                    [ phoenix::var(seconds) = phoenix::arg1 ]
                >> *boost::spirit::space_p
                >> boost::spirit::strlit< nliteral >( "+0000" )
            ) ).full ) {
        throw exceptions::not_implemented(
            "fostlib::coercer< timestamp, rfc1123_timestamp >::coerce"
                " -- parse failure");
    }
    return timestamp(year, month, day, hour, minute, seconds);
}
