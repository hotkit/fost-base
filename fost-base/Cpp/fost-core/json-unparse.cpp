/*
    Copyright 2007-2013, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/json.hpp>
#include <fost/unicode.hpp>


using namespace fostlib;



namespace {
    setting< int > c_tabwidth( L"fost-base/json-unparse.cpp", L"JSON", L"Unparse tab width", 4, true );
    string tab( std::size_t tabs ) {
        return string( c_tabwidth.value() * tabs, L' ' );
    }
    string to_hex( utf16 c, std::size_t digits = 3 ) {
        wchar_t l = ( c & 0xf ) + L'0';
        if ( l > L'9' ) l = l - L'0' - 10 + L'a';
        if ( digits )
            return to_hex( c >> 4, digits - 1 ) + string( 1, l );
        else
            return string( 1, l );
    }
    string string_to_json( const string &s ) {
        string ret( L"\"" );
        for ( string::const_iterator i( s.begin() ); i != s.end(); ++i )
            switch( *i ) {
            case L'\n': ret += L"\\n"; break;
            case L'\r': ret += L"\\r"; break;
            case L'\t': ret += L"\\t"; break;
            case L'\\': ret += L"\\\\"; break;
            case L'\"': ret += L"\\\""; break;
            default:
                if ( *i > 0x7f ) {
                    utf16 o[ 2 ];
                    std::size_t l = utf::encode( *i, o, o + 2 );
                    ret += L"\\u" + to_hex( o[0] );
                    if ( l == 2 )
                        ret += L"\\u" + to_hex( o[1] );
                } else
                    ret += *i;
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
                s << ( i == t.begin() ? string() : L"," ) << json::unparse( **i, false );
            return L"[" + string( s.str() ) + L"]";
        }
        string operator()( const json::object_t &t ) const {
            stringstream s;
            for ( json::object_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? string() : L"," ) << string_to_json( i->first ) << L":" << json::unparse( *i->second, false );
            return L"{" + string( s.str() ) + L"}";
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
            ++indentation;
            for ( json::array_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? L"\n" : L",\n" ) << tab( indentation )
                    << boost::apply_visitor( *this, **i );
            --indentation;
            return L"[" + string( s.str() ) + L"\n" + tab( indentation ) + L"]";
        }
        string operator()( const json::object_t &t ) const {
            stringstream s;
            ++indentation;
            for ( json::object_t::const_iterator i( t.begin() ); i != t.end(); ++i )
                s << ( i == t.begin() ? L"\n" : L",\n" )
                    << tab( indentation ) << string_to_json( i->first ) << L" : "
                    << boost::apply_visitor( *this, *i->second );
            --indentation;
            return L"{" + string( s.str() ) + L"\n" + tab( indentation ) + L"}";
        }
    };
}
string fostlib::json::unparse( const json &json, bool pretty ) {
    if ( pretty )
        return boost::apply_visitor( ::to_pretty(), json ) + L"\n";
    else
        return boost::apply_visitor( ::to_json(), json );
}

