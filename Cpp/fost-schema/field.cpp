/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


namespace {
    typedef library< field_base* > registry_type;
    registry_type &registry() {
        static registry_type lib;
        return lib;
    }
}


const field_wrapper< int64_t > integer( L"integer" );


fostlib::field_base::field_base( const string &n )
: type_name( n ) {
    registry().add( type_name(), this );
}
fostlib::field_base::~field_base() {
    registry().remove( type_name(), this );
}

const field_base &fostlib::field_base::fetch( const string &n ) {
    try {
        registry_type::found_t f = registry().find( n );
        if ( f.size() != 1 )
            throw exceptions::out_of_range< std::size_t >( L"There should only be one field of the given type found", 1, 1, f.size() );
        else
            return **f.begin();
    } catch ( exceptions::exception &e ) {
        e.info() << L"Fetching a field_base of type: " << n << std::endl;
        throw;
    }
}

