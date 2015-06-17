/*
    Copyright 2007-2015, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/insert.hpp>
#include <fost/json.hpp>
#include <fost/detail/coerce.hpp>
#include <fost/detail/utility.hpp>

#include <fost/exception/json_error.hpp>
#include <fost/exception/not_implemented.hpp>
#include <fost/exception/not_null.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/out_of_range.hpp>
#include <fost/exception/parse_error.hpp>


using namespace fostlib;


fostlib::jcursor::jcursor() {
}
fostlib::jcursor::jcursor( int i ) {
    m_position.push_back( i );
}
fostlib::jcursor::jcursor( json::array_t::size_type i ) {
    m_position.push_back( i );
}
fostlib::jcursor::jcursor( nliteral n ) {
    m_position.push_back( fostlib::string(n) );
}
fostlib::jcursor::jcursor( wliteral n ) {
    m_position.push_back( fostlib::string(n) );
}
fostlib::jcursor::jcursor( const string &i ) {
    m_position.push_back( i );
}
fostlib::jcursor::jcursor( const json &j ) {
    nullable< int64_t > i = j.get< int64_t >();
    if ( !i.isnull() )
        m_position.push_back( coerce< json::array_t::size_type >( i.value() ) );
    else {
        nullable< string > s = j.get< string >();
        if ( !s.isnull() )
            m_position.push_back( s.value() );
        else
            throw exceptions::json_error(
                L"The jcursor location must be a string or integer", j);
    }
}

fostlib::jcursor::jcursor( stack_t::const_iterator b, stack_t::const_iterator e )
: m_position( b, e ) {
}


jcursor fostlib::jcursor::split(const string &s, const string &c) {
    fostlib::split_type path = fostlib::split(s, c);
    fostlib::jcursor position;
    for ( fostlib::split_type::const_iterator part(path.begin());
            part != path.end(); ++part ) {
        try {
            int index = fostlib::coerce<int>(*part);
            position /= index;
        } catch ( fostlib::exceptions::parse_error& ) {
            position /= *part;
        }
    }
    return position;
}


jcursor &fostlib::jcursor::operator /= ( json::array_t::size_type i ) {
    m_position.push_back( i );
    return *this;
}
jcursor &fostlib::jcursor::operator /= ( const string &i ) {
    m_position.push_back( i );
    return *this;
}
jcursor &fostlib::jcursor::operator /= ( const json &j ) {
    return operator /= ( jcursor( j ) );
}
jcursor &fostlib::jcursor::operator /= ( const jcursor &r ) {
    m_position.insert( m_position.end(), r.m_position.begin(), r.m_position.end() );
    return *this;
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
    if ( !j.has_key( *this ) ) {
        (*this)( j ) =v;
    } else {
        exceptions::not_null error( L"There is already some JSON at this key position");
        fostlib::insert(error.data(), "json", j);
        fostlib::insert(error.data(), "value", v);
        fostlib::insert(error.data(), "key", *this);
        throw error;
    }
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

namespace {
    struct del_key : public boost::static_visitor< void > {
        json::element_t &element;
        del_key( json::element_t &j ) : element( j ) {}

        void operator()( json::array_t::size_type k ) const {
            json::array_t *arr( boost::get< json::array_t >( &element ) );
            if ( !arr )
                throw exceptions::json_error("A numeric key can only be used to delete from a JSON array");
            if ( k >= arr->size() )
                throw exceptions::out_of_range< std::size_t >("Trying to erase beyond the end of the array",
                    0, arr->size(), k
                );
            arr->erase(arr->begin() + k);
        }
        void operator()( const string &k ) const {
            json::object_t *obj( boost::get< json::object_t >( &element ) );
            if ( !obj )
                throw exceptions::json_error( L"A string key can only be deleted from JSON objects" );
            json::object_t::iterator p( obj->find( k ) );
            if ( p == obj->end() )
                throw exceptions::json_error( L"Key can't be removed from object as it doesn't exist in the object", json( *obj ) );
            obj->erase( p );
        }
    };
}
json &fostlib::jcursor::del_key( json &j ) const {
    if ( m_position.size() == 0 )
        throw exceptions::out_of_range< std::size_t >(
            L"The jcursor must have at least one level of items in it",
            1, std::numeric_limits< std::size_t >::max(), m_position.size()
        );
    jcursor move_to( m_position.begin(), --m_position.end() );
    boost::apply_visitor( ::del_key( move_to( j ).m_element ), *m_position.rbegin() );
    return j;
}


bool fostlib::jcursor::operator == ( const jcursor &j ) const {
    return m_position == j.m_position;
}
