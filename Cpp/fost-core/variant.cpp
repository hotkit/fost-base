/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"

#include <fost/core>
#include <fost/variant.hpp>


using namespace fostlib;


namespace {
    struct isnull : public boost::static_visitor< bool > {
        bool operator()( t_null ) const {
            return true;
        }
        bool operator()( const string & ) const {
            return false;
        }
        template< typename T >
        bool operator()( T ) const {
            return false;
        }
    };
}
bool fostlib::variant::isnull() const {
    return boost::apply_visitor( ::isnull(), m_v );
}


//double as_double() const;


bool fostlib::variant::operator ==( const variant &r ) const {
    return m_v == r.m_v;
}


ostream &operator <<( ostream &o, const variant &v ) {
    return o << json( v );
}
