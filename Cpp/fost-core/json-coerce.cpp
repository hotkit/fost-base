/*
    Copyright 2008-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/insert.hpp>
#include <fost/push_back.hpp>
#include <fost/exception/cast_fault.hpp>
#include <fost/exception/json_error.hpp>
#include <fost/exception/not_a_number.hpp>


using namespace fostlib;


/*
    as_bool
*/
namespace {
    struct as_bool : public boost::static_visitor< bool > {
        bool operator ()( const json::atom_t &t ) const {
            return coerce< bool >( t );
        }
        bool operator()( const json::array_p &a ) const {
            return a->size();
        }
        bool operator()( const json::object_p &o ) const {
            return o->size();
        }
    };
}
bool fostlib::coercer< bool, json >::coerce( const json &j ) {
    return boost::apply_visitor( ::as_bool(), j );
}


/*
    double
*/
namespace {
    struct as_double : public boost::static_visitor< double > {
        double operator ()( const json::atom_t &t ) const {
            return coerce< double >( t );
        }
        double operator ()( const json::array_p & ) const {
            throw fostlib::exceptions::not_a_number( L"Array cannot convert to a number" );
        }
        double operator ()( const json::object_p & ) const {
            throw fostlib::exceptions::not_a_number( L"Object cannot convert to a number" );
        }
    };
}
double fostlib::coercer< double, json >::coerce( const json &j ) {
    return boost::apply_visitor( ::as_double(), j );
}


/*
    as_wstring
*/
namespace {
    struct as_string : public boost::static_visitor< string > {
        string operator ()( const json::atom_t &t ) const {
            return coerce< string >( t );
        }
        string operator () (const json::array_p &a) const {
            fostlib::exceptions::cast_fault error("Cannot convert a JSON array to a string");
            fostlib::insert(error.data(), "array", *a);
            throw error;
        }
        string operator () (const json::object_p &o) const {
            fostlib::exceptions::cast_fault error("Cannot convert a JSON object to a string");
            fostlib::insert(error.data(), "object", *o);
            throw error;
        }
    };
}
string fostlib::coercer< string, json >::coerce( const json &j ) {
    return boost::apply_visitor( ::as_string(), j );
}


/*
    jcursor
*/

namespace {
    struct jc_as_js : public boost::static_visitor< json > {
        json operator() ( json::array_t::size_type p ) const {
            return json(coerce<int64_t>(p));
        }
        json operator() ( const string &p ) const {
            return json(p);
        }
    };
}
json fostlib::coercer< json, jcursor >::coerce(const jcursor &j) {
    fostlib::json cursor;
    for ( jcursor::const_iterator p(j.begin()); p != j.end(); ++p )
        push_back(cursor, boost::apply_visitor( ::jc_as_js(), *p) );
    return cursor;
}


jcursor fostlib::coercer< jcursor, json >::coerce(const json &j) {
    jcursor ret;
    if ( j.isnull() )
        return ret;
    else if ( !j.isarray() )
        throw exceptions::json_error(
            "The JSON must contain an array to convertible to a jcursor", j);
    for ( std::size_t i(0); j.has_key(i); ++i )
        ret /= j[i];
    return ret;
}

