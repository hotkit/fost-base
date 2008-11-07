/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/variant.hpp>
#include <fost/exception/null.hpp>
#include <fost/exception/out_of_range.hpp>


using namespace fostlib;


/*
    bool
*/
namespace {
    struct as_bool : public boost::static_visitor< bool > {
        bool operator()( t_null ) const {
            return false;
        }
        bool operator()( bool b ) const {
            return b;
        }
        bool operator()( int64_t i ) const {
            return i;
        }
        bool operator()( double d ) const {
            return d != 0.;
        }
        bool operator()( const string &s ) const {
            return !s.empty();
        }
    };
}
bool fostlib::coercer< bool, variant >::coerce( const variant &v ) {
    return boost::apply_visitor( ::as_bool(), v );
}


/*
    int64_t
*/
namespace {
    struct as_int64_t : public boost::static_visitor< int64_t > {
        int64_t operator()( t_null ) const {
            throw fostlib::exceptions::null( L"Cannot convert null to int64_t" );
        }
        int64_t operator()( bool b ) const {
            return b ? 1 : 0;
        }
        int64_t operator()( int64_t i ) const {
            return i;
        }
        int64_t operator()( double d ) const {
            if ( d > std::numeric_limits< int64_t >::max() || d < std::numeric_limits< int64_t >::min() )
                throw fostlib::exceptions::out_of_range< int64_t, double >( std::numeric_limits< int64_t >::min(), std::numeric_limits< int64_t >::max(), d );
            else if ( d >= 0 )
                return int64_t( d + .5 );
            else
                return int64_t( d - .5 );
        }
        int64_t operator()( const string &s ) const {
            return coerce< int64_t >( s );
        }
    };
}
int64_t fostlib::coercer< int64_t, variant >::coerce( const variant &v ) {
    return boost::apply_visitor( ::as_int64_t(), v );
}


/*
    double
*/
namespace {
    struct as_double : public boost::static_visitor< double > {
        double operator()( t_null ) const {
            throw fostlib::exceptions::null( L"Cannot convert null to double" );
        }
        double operator()( bool b ) const {
            return b ? 1 : 0;
        }
        double operator()( int64_t i ) const {
            return double( i );
        }
        double operator()( double d ) const {
            return d;
        }
        double operator()( const string &s ) const {
            return coerce< double >( s );
        }
    };
}
double fostlib::coercer< double, variant >::coerce( const variant &v ) {
    return boost::apply_visitor( ::as_double(), v );
}


/*
    string
*/
namespace {
    struct as_string : public boost::static_visitor< string > {
        string operator()( t_null ) const {
            throw fostlib::exceptions::null( L"Cannot convert null to string" );
        }
        string operator()( bool b ) const {
            return coerce< string> ( b );
        }
        string operator()( int64_t i ) const {
            return coerce< string >( i );
        }
        string operator()( double d ) const {
            return coerce< string >( d );
        }
        string operator()( const string &s ) const {
            return s;
        }
    };
}
string fostlib::coercer< string, variant >::coerce( const variant &v ) {
    return boost::apply_visitor( ::as_string(), v );
}
