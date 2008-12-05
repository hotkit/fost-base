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
    fostlib::enclosure
*/


const enclosure enclosure::global( L"" );

fostlib::enclosure::enclosure( const string &n )
: name( n ), m_parent( NULL ) {
}
fostlib::enclosure::enclosure( const enclosure &e, const string &n )
: name( n ), m_parent( &e ) {
}

string fostlib::enclosure::fq_name( const string &delim ) const {
    if ( m_parent )
        return m_parent->fq_name( delim ) + delim + name();
    else
        return name();
}
const enclosure &fostlib::enclosure::parent() const {
    return m_parent ? *m_parent : global;
}


/*
    fostlib::meta_instance
*/

fostlib::meta_instance::meta_instance( const string &n )
: enclosure( n ) {
}
fostlib::meta_instance::meta_instance( const enclosure &e, const string &n )
: enclosure( e, n ) {
}

namespace {
    boost::shared_ptr< meta_attribute > make_attribute(
        const string &name, const string &type, bool key, bool not_null,
        const nullable< std::size_t > &size, const nullable< std::size_t > &precision
    ) {
        boost::shared_ptr< meta_attribute > attr(  field_base::fetch( type ).meta_maker(
            name, key, not_null, size, precision
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
    if ( find_attr( m_columns.begin(), m_columns.end(), name ) != m_columns.end() )
        throw exceptions::null( L"Cannot have two attributes with the same name" );
    m_columns.push_back( make_attribute( name, type, true, true, size, precision ) );
    return *this;
}
meta_instance &fostlib::meta_instance::field(
    const string &name, const string &type, bool not_null,
    const nullable< std::size_t > &size, const nullable< std::size_t > &precision
) {
    if ( find_attr( m_columns.begin(), m_columns.end(), name ) != m_columns.end() )
        throw exceptions::null( L"Cannot have two attributes with the same name" );
    m_columns.push_back( make_attribute( name, type, false, not_null, size, precision ) );
    return *this;
}

const meta_attribute &fostlib::meta_instance::operator[] ( const string &n ) const {
    columns_type::const_iterator p( find_attr( m_columns.begin(), m_columns.end(), n ) );
    if ( p != m_columns.end() )
        return **p;
    else
        throw exceptions::null( L"Could not find attribute definition", n );
}

boost::shared_ptr< instance > fostlib::meta_instance::create() const {
    return create( json() );
}
boost::shared_ptr< instance > fostlib::meta_instance::create( const json &j ) const {
    const json empty, &v( j.isobject() ? j : empty );
    boost::shared_ptr< instance > object( new instance( *this ) );
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

