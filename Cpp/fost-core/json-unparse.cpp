/*
    Copyright 2007-2016, Felspar Co Ltd. http://support.felspar.com/
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
    inline void tab(std::string &into, std::size_t tabs) {
        const std::size_t chars = coerce<std::size_t>(c_tabwidth.value());
        for ( std::size_t t{0}; t != tabs * chars; ++t )
            into += ' ';
    }
    inline void to_hex(std::string &into, utf16 c, std::size_t digits = 3) {
        wchar_t l = ( c & 0xf ) + L'0';
        if ( l > L'9' ) l = l - L'0' - 10 + L'a';
        if ( digits )
            to_hex(into, c >> 4, digits - 1);
        into += l;
    }
    inline void string_to_json(std::string &into, const string &s) {
        into += '"';
        for ( string::const_iterator i(s.begin()); i != s.end(); ++i ) {
            switch( *i ) {
            case L'\n': into += "\\n"; break;
            case L'\r': into += "\\r"; break;
            case L'\t': into += "\\t"; break;
            case L'\\': into += "\\\\"; break;
            case L'\"': into += "\\\""; break;
            default:
                if ( *i > 0x7f ) {
                    utf16 o[2];
                    std::size_t l = utf::encode(*i, o, o + 2);
                    into += "\\u";
                    to_hex(into, o[0]);
                    if ( l == 2 ) {
                        into += "\\u";
                        to_hex(into, o[1]);
                    }
                } else {
                    into += *i;
                }
            }
        }
        into += '\"';
    }


    struct atom_to_json : public boost::static_visitor<void> {
        std::string &into;

        atom_to_json(std::string &i)
        : into(i) {
        }

        void operator() (t_null) const {
            into += "null";
        }
        void operator() (bool b) const {
            if ( b ) {
                into += "true";
            } else {
                into += "false";
            }
        }
        void operator () (int64_t v) const {
            // TODO: Go back to the below line
//             into += std::to_string(v);
            char buffer[50];
            snprintf(buffer, sizeof(buffer), "%lli", v);
            into += buffer;
        }
        void operator() (const string &s) const {
            string_to_json(into, s);
        }
        template< typename T >
        void operator() (T i) const {
            into += coerce<string>(i).std_str();
        }
    };


    struct to_json : public boost::static_visitor<void> {
        std::string &into;

        to_json(std::string &i)
        : into(i) {
        }

        void operator()( const json::atom_t &t ) const {
            boost::apply_visitor(atom_to_json(into), t);
        }
        void operator()( const json::array_t &t ) const {
            into += '[';
            for ( json::array_t::const_iterator i( t.begin() ); i != t.end(); ++i ) {
                into += (i == t.begin() ? "" : "," );
                json::unparse(into, **i, false);
            }
            into += ']';
        }
        void operator()( const json::object_t &t ) const {
            into += '{';
            for ( json::object_t::const_iterator i( t.begin() ); i != t.end(); ++i ) {
                into += ( i == t.begin() ? "" : "," );
                string_to_json(into, i->first);
                into += ':';
                json::unparse(into, *i->second, false);
            }
            into += '}';
        }
    };


    struct to_pretty : public boost::static_visitor<void> {
        std::string &into;
        mutable std::size_t indentation;

        to_pretty(std::string &i)
        : into(i), indentation( 0 ) {
        }

        void operator() (const json::atom_t &t) const {
            boost::apply_visitor(atom_to_json{into}, t);
        }
        void operator() (const json::array_t &t) const {
            into += '[';
            ++indentation;
            for ( json::array_t::const_iterator i( t.begin() ); i != t.end(); ++i ) {
                into += ( i == t.begin() ? "\n" : ",\n" );
                tab(into, indentation);
                boost::apply_visitor(*this, **i);
            }
            --indentation;
            into += '\n';
            tab(into, indentation);
            into += ']';
        }
        void operator() (const json::object_t &t) const {
            into += '{';
            ++indentation;
            for ( json::object_t::const_iterator i( t.begin() ); i != t.end(); ++i ) {
                into += ( i == t.begin() ? "\n" : ",\n" );
                tab(into, indentation);
                string_to_json(into, i->first);
                into += " : ";
                boost::apply_visitor(*this, *i->second);
            }
            --indentation;
            into += '\n';
            tab(into, indentation);
            into += '}';
        }
    };


}


void fostlib::json::unparse(std::string &into, const json &json, bool pretty) {
    if ( pretty ) {
        boost::apply_visitor(::to_pretty(into), json);
        into += '\n';
    } else {
        boost::apply_visitor(::to_json(into), json);
    }
}

