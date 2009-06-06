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
: m_instance( factory.meta()->create( dbc, j ) ) {
}

fostlib::model_base::~model_base() {
}

instance &fostlib::model_base::meta() {
    return *m_instance;
}


/*
    fostlib::model_base::factory_base
*/

fostlib::model_base::factory_base::factory_base( const string &name )
: name( name ), m_ns( enclosure::global ) {
}

fostlib::model_base::factory_base::factory_base( const enclosure &ns, const string &name )
: name( name ), m_ns( ns ) {
}

const enclosure &fostlib::model_base::factory_base::ns() const {
    return m_ns;
}

boost::shared_ptr< meta_instance > fostlib::model_base::factory_base::meta() const {
    if ( !m_meta.get() )
        m_meta = boost::shared_ptr< meta_instance >(
            new meta_instance( ns(), name() )
        );
    return m_meta;
}
