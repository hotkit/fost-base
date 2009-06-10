/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>
#include <fost/db.hpp>
#include <fost/exception/null.hpp>


using namespace fostlib;


/*
    fostlib::meta_attribute
*/

fostlib::meta_attribute::meta_attribute(
    const string &name, const field_base &type, bool key, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) : name( name ), key( key ), not_null( not_null ), size( size ), precision( precision ), m_type( type ) {
}

const field_base &meta_attribute::type() const {
    return m_type;
}


/*
    fostlib::attribute_base
*/

fostlib::attribute_base::attribute_base( const meta_attribute &m )
: m_meta( m ) {
}

#include <fost/exception/not_implemented.hpp>
const meta_attribute &fostlib::attribute_base::_meta() const {
    return m_meta;
}

