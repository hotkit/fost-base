/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>


using namespace fostlib;


fostlib::meta_instance::meta_instance( const string &n )
: name( n ) {
}

string fostlib::meta_instance::table( const instance_base & ) const {
    return name();
}


#include <fost/exception/not_implemented.hpp>
namespace {
    boost::shared_ptr< meta_attribute > attribute(
        const string &name, const string &type, bool not_null,
        const nullable< std::size_t > &size, const nullable< std::size_t > &precision
    ) {
        boost::shared_ptr< meta_attribute > attr( new meta_attribute(
            name, field_base::fetch( type ), not_null, size, precision
        ) );
        return attr;
    }
}
meta_instance &fostlib::meta_instance::primary_key(
    const string &name, const string &type,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) {
    keys.push_back( attribute( name, type, false, size, precision ) );
    return *this;
}
meta_instance &fostlib::meta_instance::field(
    const string &name, const string &type, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) {
    columns.push_back( attribute( name, type, not_null, size, precision ) );
    return *this;
}


fostlib::meta_attribute::meta_attribute(
    const string &name, const field_base &type, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) : name( name ), m_type( type ), not_null( not_null ), size( size ), precision( precision ) {
}

const field_base &meta_attribute::type() const {
    return m_type;
}
