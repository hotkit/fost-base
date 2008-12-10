/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <boost/lambda/lambda.hpp>


using namespace fostlib;



namespace {
    string string_to_json( const string &s ) {
        string ret( L"\"" );
        for ( string::const_iterator i( s.begin() ); i != s.end(); ++i )
            switch( *i ) {
            case L'\n': ret += L"\\n"; break;
            case L'\t': ret += L"\\t"; break;
            case L'\\': ret += L"\\\\"; break;
            case L'\"': ret += L"\\\""; break;
            default: ret += *i;
            }
        return ret + L'\"';
    }
    struct atom_to_json : public boost::static_visitor< string > {
        string operator()( t_null ) const {
            return L"null";
        }
        string operator()( const string &s ) const {
            return string_to_json( s );
        }
        template< typename T >
        string operator()( T i ) const {
            return coerce< string >( i );
        }
    };
    struct to_json : public boost::static_visitor< string > {
        string operator()( const json::atom_t &t ) const {
            return boost::apply_visitor( atom_to_json(), t );
        }
        string operator()( const json::array_t &t ) const {
            stringstream s;
            for ( json::array_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? string() : L"," ) << json::unparse( **i );
            return L"[" + s.str() + L"]";
        }
        string operator()( const json::object_t &t ) const {
            stringstream s;
            for ( json::object_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? string() : L"," ) << string_to_json( i->first ) << L":" << json::unparse( *i->second );
            return L"{" + s.str() + L"}";
        }
    };
    struct to_pretty : public boost::static_visitor< string > {
        mutable std::size_t indentation;
        to_pretty() : indentation( 0 ) {}

        string operator()( const json::atom_t &t ) const {
            return boost::apply_visitor( atom_to_json(), t );
        }
        string operator()( const json::array_t &t ) const {
            stringstream s;
            indentation += 2;
            for ( json::array_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? L"\n" : L",\n" ) << string( indentation, L' ' )
                    << boost::apply_visitor( *this, **i );
            indentation -= 2;
            return L"[" + s.str() + L"\n" + string( indentation, L' ' ) + L"]";
        }
        string operator()( const json::object_t &t ) const {
            stringstream s;
            indentation += 2;
            for ( json::object_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? L"\n" : L",\n" )
                    << string( indentation, L' ' ) << string_to_json( i->first ) << L" : "
                    << boost::apply_visitor( *this, *i->second );
            indentation -= 2;
            return L"{" + s.str() + L"\n" + string( indentation, L' ' ) + L"}";
        }
    };
}
string fostlib::json::unparse( const json &json, bool pretty ) {
    if ( pretty )
        return boost::apply_visitor( ::to_pretty(), json ) + L"\n";
    else
        return boost::apply_visitor( ::to_json(), json );
}

