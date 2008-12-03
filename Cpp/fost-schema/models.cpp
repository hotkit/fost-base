/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>
#include <fost/exception/null.hpp>


using namespace fostlib;


/*
    fostlib::meta_instance
*/

fostlib::meta_instance::meta_instance( const string &n )
: name( n ) {
}

namespace {
    boost::shared_ptr< meta_attribute > make_attribute(
        const string &name, const string &type, bool not_null,
        const nullable< std::size_t > &size, const nullable< std::size_t > &precision
    ) {
        boost::shared_ptr< meta_attribute > attr( new meta_attribute(
            name, field_base::fetch( type ), not_null, size, precision
        ) );
        return attr;
    }
    template< typename const_iterator >
    const_iterator find_attr( const_iterator p, const const_iterator end, const string &n ) {
        for( ; p != end; ++p )
            if ( (*p)->name() == n )
                return p;
        return end;
    }
}
meta_instance &fostlib::meta_instance::primary_key(
    const string &name, const string &type,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) {
    if ( find_attr( keys.begin(), keys.end(), name ) != keys.end() ||
        find_attr( columns.begin(), columns.end(), name ) != columns.end()
    ) throw exceptions::null( L"Cannot have two attributes with the same name" );
    keys.push_back( make_attribute( name, type, false, size, precision ) );
    return *this;
}
meta_instance &fostlib::meta_instance::field(
    const string &name, const string &type, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) {
    if ( find_attr( keys.begin(), keys.end(), name ) != keys.end() ||
        find_attr( columns.begin(), columns.end(), name ) != columns.end()
    ) throw exceptions::null( L"Cannot have two attributes with the same name" );
    columns.push_back( make_attribute( name, type, not_null, size, precision ) );
    return *this;
}

const meta_attribute &fostlib::meta_instance::operator[] ( const string &n ) const {
    columns_type::const_iterator p( find_attr( keys.begin(), keys.end(), n ) );
    if ( p != keys.end() )
        return **p;
    else
        p = find_attr( columns.begin(), columns.end(), n );
    if ( p == columns.end() )
        throw exceptions::null( L"Could not find attribute definition", n );
    else
        return **p;
}

boost::shared_ptr< instance > fostlib::meta_instance::operator() () const {
    return boost::shared_ptr< instance >( new instance( *this ) );
}

string fostlib::meta_instance::table( const instance & ) const {
    return name();
}


/*
    fostlib::meta_attribute
*/

fostlib::meta_attribute::meta_attribute(
    const string &name, const field_base &type, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) : name( name ), not_null( not_null ), size( size ), precision( precision ), m_type( type ) {
}

const field_base &meta_attribute::type() const {
    return m_type;
}


/*
    fostlib::attribute
*/

#include <fost/exception/not_implemented.hpp>
const meta_attribute &fostlib::attribute::_meta() const {
    throw exceptions::not_implemented( L"fostlib::attribute::_meta() const" );
}


/*
    fostlib::instance
*/

fostlib::instance::instance( const meta_instance &meta )
: m_meta( meta ) {
}

const meta_instance &fostlib::instance::_meta() const {
    return m_meta;
}
#include <fost/exception/not_implemented.hpp>
attribute &fostlib::instance::operator [] ( const string &name ) {
    throw exceptions::not_implemented( L"fostlib::instance::operator [] ( const string &name )" );
}

