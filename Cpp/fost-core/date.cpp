/*
    Copyright 2000-2014, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/date.hpp>
#include <fost/timediff.hpp>


using namespace fostlib;


string fostlib::coercer< string, date >::coerce( date d ) {
    return fostlib::coerce< string >( fostlib::coerce< ascii_string >(
        boost::gregorian::to_iso_extended_string(fostlib::coerce< boost::gregorian::date >( d ))
    ) );
}

date fostlib::coercer< date, json >::coerce( const json &j ) {
    nullable< string > s( j.get< string >() );
    if ( s.isnull() )
        throw exceptions::not_implemented("fostlib::coercer< date, json >::coerce( const json &j ) -- with non-string JSON");
    return date( boost::gregorian::from_string( fostlib::coerce< ascii_string >( s.value() ).underlying() ) );
}
