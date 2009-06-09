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
    fostlib::model_base
*/

fostlib::model_base::model_base( const factory_base &factory, dbconnection &dbc, const json &j )
: m_instance( factory._meta()->create( dbc, j ) ) {
}

fostlib::model_base::~model_base() {
}

instance &fostlib::model_base::_instance() {
    return *m_instance;
}


/*
    fostlib::model_base::factory_base
*/

namespace {
    typedef std::vector< const model_base::factory_base * > factory_list_type;
    boost::mutex &g_mutex() {
        static boost::mutex m;
        return m;
    }
    factory_list_type &g_factory_list() {
        static factory_list_type *v = NULL;
        if ( !v )
            v = new factory_list_type;
        return *v;
    }
    void cleanup_factories() {
        boost::mutex::scoped_lock lock( g_mutex() );
        factory_list_type &factories = g_factory_list();
        for ( factory_list_type::const_iterator i( factories.begin() ); i != factories.end(); ++i )
            delete *i;
        delete &factories;
    }
    void register_factory( const model_base::factory_base *factory ) {
        boost::mutex::scoped_lock lock( g_mutex() );
        factory_list_type &factories = g_factory_list();
        if ( g_factory_list().size() == 0 )
            atexit( cleanup_factories );
        factories.push_back( factory );
    }
}

fostlib::model_base::factory_base::factory_base( const string &name )
: name( name ), m_container( &enclosure::global ) {
    register_factory( this );
}
fostlib::model_base::factory_base::factory_base( const enclosure &ns, const string &name )
: name( name ), m_container( &ns ) {
    register_factory( this );
}
fostlib::model_base::factory_base::factory_base( const factory_base *ns, const string &name )
: name( name ), m_container( ns ) {
    register_factory( this );
}

namespace {
    const struct container_content : public boost::static_visitor< const enclosure & >{
        const enclosure &operator () ( const enclosure * const enc ) const {
            return *enc;
        }
        const enclosure &operator () ( const model_base::factory_base * const enc ) const {
            return *enc->_meta();
        }
    } c_container_dereferencer;
}
const enclosure &fostlib::model_base::factory_base::ns() const {
    return boost::apply_visitor( c_container_dereferencer, m_container );
}

boost::shared_ptr< meta_instance > fostlib::model_base::factory_base::_meta() const {
    if ( !m_meta.get() ) {
        m_meta = boost::shared_ptr< meta_instance >(
            new meta_instance( ns(), name() )
        );
        attributes_type::keys_t keys = m_attributes.keys();
        for ( attributes_type::keys_t::const_iterator k( keys.begin() ); k != keys.end(); ++k ) {
            attributes_type::found_t values = m_attributes.find( *k );
            for ( attributes_type::found_t::const_iterator a( values.begin() ); a != values.end(); ++a )
                m_meta->primary_key( (*a)->name(), L"integer" );
        }
    }
    return m_meta;
}


/*
    fostlib::model_base::attribute_binding_base
*/

fostlib::model_base::attribute_binding_base::attribute_binding_base( const factory_base &factory, const string &name )
: name( name ) {
    factory.m_attributes.add( name, this );
}

