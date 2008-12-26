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
#include <fost/exception/not_null.hpp>
#include <fost/exception/null.hpp>


using namespace fostlib;


fostlib::jcursor::jcursor() {
}

fostlib::jcursor::jcursor( stack_t::const_iterator b, stack_t::const_iterator e )
: m_position( b, e ) {
}

jcursor fostlib::jcursor::operator[]( json::array_t::size_type i ) const {
    jcursor p( *this );
    p.m_position.push_back( i );
    return p;
}
jcursor fostlib::jcursor::operator[]( const string &i ) const {
    jcursor p( *this );
    p.m_position.push_back( i );
    return p;
}
jcursor fostlib::jcursor::operator[]( const json &j ) const {
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

jcursor &fostlib::jcursor::enter() {
    m_position.push_back( 0 );
    return *this;
}
jcursor &fostlib::jcursor::enter( const string &i ) {
    m_position.push_back( i );
    return *this;
}
jcursor &fostlib::jcursor::pop() {
    m_position.pop_back();
    return *this;
}

jcursor &fostlib::jcursor::operator ++() {
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
json &fostlib::jcursor::operator() ( json &j ) const {
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


json &fostlib::jcursor::push_back( json &j, const json &v ) const {
    json &array = (*this)( j );
    if ( array.isnull() ) {
        json::array_t na;
        na.push_back( boost::shared_ptr< json >( new json( v ) ) );
        array = na;
    } else if ( array.isarray() )
        boost::get< json::array_t >( array.m_element ).push_back( boost::shared_ptr< json >( new json( v ) ) );
    else
        throw exceptions::json_error( L"Can only push onto the back of a JSON array" );
    return j;
}

json &fostlib::jcursor::insert( json &j, const json &v ) const {
    if ( !j.has_key( *this ) )
        (*this)( j ) =v;
    else
        throw exceptions::not_null( L"There is already some JSON at this key position",
            json::unparse( j, true ) + L"\n" + json::unparse( v, true )
        );
    return j;
}

json &fostlib::jcursor::replace( json &j, const json &v ) const {
    if ( j.has_key( *this ) )
        (*this)( j ) = v;
    else
        throw exceptions::null( L"There is nothing to replace at this key position",
            json::unparse( j, true ) + L"\n" + json::unparse( v, true )
        );
    return j;
}
