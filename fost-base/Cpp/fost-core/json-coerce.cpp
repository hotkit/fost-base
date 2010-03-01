/*
    Copyright 2008-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/exception/not_a_number.hpp>
#include <fost/exception/cast_fault.hpp>


using namespace fostlib;


/*
    as_bool
*/
namespace {
    struct as_bool : public boost::static_visitor< bool > {
        bool operator ()( const json::atom_t &t ) const {
            return coerce< bool >( t );
        }
        bool operator()( const json::array_t &a ) const {
            return a.size();
        }
        bool operator()( const json::object_t &o ) const {
            return o.size();
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
        double operator ()( const json::array_t & ) const {
            throw fostlib::exceptions::not_a_number( L"Array cannot convert to a number" );
        }
        double operator ()( const json::object_t & ) const {
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
        string operator ()( const json::array_t & ) const {
            throw fostlib::exceptions::cast_fault( L"Cannot convert a JSON array to a string" );
        }
        string operator ()( const json::object_t & ) const {
            throw fostlib::exceptions::cast_fault( L"Cannot convert a JSON object to a string" );
        }
    };
}
string fostlib::coercer< string, json >::coerce( const json &j ) {
    return boost::apply_visitor( ::as_string(), j );
}
