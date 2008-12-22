/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <fost/string/coerce.hpp>

#include <fost/exception/json_error.hpp>
#include <fost/exception/not_implemented.hpp>
#include <fost/exception/null.hpp>


using namespace fostlib;


fostlib::jcursor::jcursor() {
}

fostlib::jcursor::jcursor( stack_t::const_iterator b, stack_t::const_iterator e )
: m_position( b, e ) {
}

jcursor jcursor::operator[]( json::array_t::size_type i ) const {
    jcursor p( *this );
    p.m_position.push_back( i );
    return p;
}
jcursor jcursor::operator[]( const string &i ) const {
    jcursor p( *this );
    p.m_position.push_back( i );
    return p;
}
jcursor jcursor::operator[]( const json &j ) const {
    nullable< int64_t > i = j.get< int64_t >();
    if ( !i.isnull() )
        return (*this)[ coerce< json::array_t::size_type >( i.value() ) ];
    nullable< string > s = j.get< string >();
    if ( !s.isnull() )
        return (*this)[ s.value() ];
    throw exceptions::not_implemented(
        L"jcursor::operator[]( const json &j ) const -- for non string or integer",
        json::unparse( j, true )
    );
}

jcursor &jcursor::enter() {
    m_position.push_back( 0 );
    return *this;
}
jcursor &jcursor::enter( const string &i ) {
    m_position.push_back( i );
    return *this;
}
jcursor &jcursor::pop() {
    m_position.pop_back();
    return *this;
}

jcursor &jcursor::operator ++() {
    if ( m_position.empty() )
        throw fostlib::exceptions::null( L"cannot increment an empty jcursor" );
    else if ( boost::get< json::array_t::size_type >( &*m_position.rbegin() ) == NULL )
        throw fostlib::exceptions::null( L"the current jcursor isn't into an array position" );
    else
        ++boost::get< json::array_t::size_type >( *m_position.rbegin() );
    return *this;
}


namespace {
    struct take_step : public boost::static_visitor< json * > {
        json::element_t &element; bool isnull;
        take_step( json::element_t &j, bool n ) : element( j ), isnull( n ) {}

        json *operator()( json::array_t::size_type k ) const {
            if ( isnull )
                element = json::array_t();
            if ( !boost::get< json::array_t >( &element ) )
                throw fostlib::exceptions::json_error(
                    L"Cannot walk through a JSON object/value which is not an array using an integer key", json( element )
                );
            json::array_t &array = boost::get< json::array_t >( element );
            while ( array.size() <= k )
                array.push_back( boost::shared_ptr< json >( new json ) );
            boost::shared_ptr< json > n( new json( *array[ k ] ) );
            array[ k ] = n;
            return n.get();
        }
        json *operator()( const string &k ) const {
            if ( isnull )
                element = json::object_t();
            if ( !boost::get< json::object_t >( &element ) )
                throw fostlib::exceptions::json_error(
                    L"Cannot walk through a JSON array/value which is not an object using a string key", json( element )
                );
            json::object_t &object = boost::get< json::object_t >( element );
            if ( object.find( k ) == object.end() )
                object.insert( std::make_pair( k, boost::shared_ptr< json >( new json ) ) );
            boost::shared_ptr< json > n( new json( *object[ k ] ) );
            object[ k ] = n;
            return n.get();
        }
    };
}
json &jcursor::operator() ( json &j ) const {
    try {
        json *loc = &j;
        for ( stack_t::const_iterator p( m_position.begin() ); p != m_position.end(); ++p )
            loc = boost::apply_visitor( take_step( loc->m_element, loc->isnull() ), *p );
        return *loc;
    } catch ( exceptions::exception &e ) {
        e.info() << L"JCursor: " /*<< coerce< string >( *this )*/ << std::endl
            << L"Traversing: " << json::unparse( j, true );
        throw;
    }
}
