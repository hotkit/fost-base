/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema.hpp"
#include <fost/schema.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/no_attribute.hpp>


using namespace fostlib;


/*
    fostlib::enclosure
*/

fostlib::enclosure::enclosure( const string &n )
: name( n ) {
}

/*
    fostlib::meta_instance
*/

fostlib::meta_instance::meta_instance( const string &n )
: enclosure( n ) {
}

namespace {
    boost::shared_ptr< meta_attribute > make_attribute(
        const string &name, const string &type, bool not_null,
        const nullable< std::size_t > &size, const nullable< std::size_t > &precision
    ) {
        boost::shared_ptr< meta_attribute > attr(  field_base::fetch( type ).meta_maker(
            name, not_null, size, precision
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
    if ( find_attr( m_keys.begin(), m_keys.end(), name ) != m_keys.end() ||
        find_attr( m_columns.begin(), m_columns.end(), name ) != m_columns.end()
    ) throw exceptions::null( L"Cannot have two attributes with the same name" );
    m_keys.push_back( make_attribute( name, type, true, size, precision ) );
    return *this;
}
meta_instance &fostlib::meta_instance::field(
    const string &name, const string &type, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) {
    if ( find_attr( m_keys.begin(), m_keys.end(), name ) != m_keys.end() ||
        find_attr( m_columns.begin(), m_columns.end(), name ) != m_columns.end()
    ) throw exceptions::null( L"Cannot have two attributes with the same name" );
    m_columns.push_back( make_attribute( name, type, not_null, size, precision ) );
    return *this;
}

const meta_attribute &fostlib::meta_instance::operator[] ( const string &n ) const {
    columns_type::const_iterator p( find_attr( m_keys.begin(), m_keys.end(), n ) );
    if ( p != m_keys.end() )
        return **p;
    else
        p = find_attr( m_columns.begin(), m_columns.end(), n );
    if ( p == m_columns.end() )
        throw exceptions::null( L"Could not find attribute definition", n );
    else
        return **p;
}

boost::shared_ptr< instance > fostlib::meta_instance::create() const {
    return create( json() );
}
boost::shared_ptr< instance > fostlib::meta_instance::create( const json &j ) const {
    const json empty, &v( j.isobject() ? j : empty );
    boost::shared_ptr< instance > object( new instance( *this ) );
    for ( columns_type::const_iterator col( m_keys.begin() ); col != m_keys.end(); ++col )
        if ( v.has_key( (*col)->name() ) )
            object->attribute( (*col)->construct( v[ (*col)->name() ] ) );
        else
            object->attribute( (*col)->construct() );
    for ( columns_type::const_iterator col( m_columns.begin() ); col != m_columns.end(); ++col )
        if ( v.has_key( (*col)->name() ) )
            object->attribute( (*col)->construct( v[ (*col)->name() ] ) );
        else
            object->attribute( (*col)->construct() );
    return object;
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
    fostlib::attribute_base
*/

fostlib::attribute_base::attribute_base( const meta_attribute &m )
: m_meta( m ) {
}

#include <fost/exception/not_implemented.hpp>
const meta_attribute &fostlib::attribute_base::_meta() const {
    return m_meta;
}


/*
    fostlib::instance
*/

fostlib::instance::instance( const meta_instance &meta )
: m_meta( meta ) {
}

void fostlib::instance::attribute( boost::shared_ptr< attribute_base > attr ) {
    m_attributes.insert( std::make_pair( attr->_meta().name(), attr ) );
}

const meta_instance &fostlib::instance::_meta() const {
    return m_meta;
}
attribute_base &fostlib::instance::operator [] ( const string &name ) {
    attributes_type::iterator p( m_attributes.find( name ) );
    if ( p == m_attributes.end() )
        throw exceptions::not_implemented( _meta().name() + L"." + name );
    else
        return *p->second;
}

