/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_JSON_HPP
#define FOST_PARSE_JSON_HPP


#include <fost/json.hpp>
#include <fost/parse/parse.hpp>


namespace fostlib {


    namespace detail {


        struct json_closure : boost::spirit::closure< json_closure, fostlib::json, fostlib::string, fostlib::json > {
            member1 jvalue;
            member2 key;
            member3 value;
        };


    }


    extern const struct json_string_parser : public boost::spirit::grammar<
        json_string_parser, string_builder_closure::context_t
    > {
        template< typename scanner_t >
        struct definition {
            definition( json_string_parser const& self ) {
                top = string[ self.text = phoenix::arg1 ];
                string =
                        boost::spirit::chlit< wchar_t >( L'"' )
                        >> *(
                            ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'\"' )[ detail::push_back( string.buffer, L'"' ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'\\' )[ detail::push_back( string.buffer, L'\\' ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'/' )[ detail::push_back( string.buffer, L'/' ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'b' )[ detail::push_back( string.buffer, 0x08 ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'f' )[ detail::push_back( string.buffer, 0x0c ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'n' )[ detail::push_back( string.buffer, L'\n' ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'r' )[ detail::push_back( string.buffer, L'\r' ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L't' )[ detail::push_back( string.buffer, L'\t' ) ]
                            | ( boost::spirit::chlit< wchar_t >( L'\\' ) >> L'u' >> boost::spirit::uint_parser< wchar_t, 16, 4, 4 >()[ detail::push_back( string.buffer, phoenix::arg1 ) ] )
                            | ( boost::spirit::anychar_p[ string.character = phoenix::arg1 ]
                                    - ( boost::spirit::chlit< wchar_t >( L'"' ) | boost::spirit::chlit< wchar_t >( L'\\' ) )
                                )[ detail::push_back( string.buffer, string.character ) ]
                        ) >> boost::spirit::chlit< wchar_t >( L'"' )[ string.text = string.buffer ];
            }
            boost::spirit::rule< scanner_t, string_builder_closure::context_t > string;
            boost::spirit::rule< scanner_t > top;

            boost::spirit::rule< scanner_t > const &start() const { return top; }
        };
    } json_string_p;


    extern const struct json_parser : public boost::spirit::grammar< json_parser, detail::json_closure::context_t > {
        template< typename scanner_t >
        struct definition {
            definition( json_parser const& self ) {
                top = *boost::spirit::space_p >> json_r[ self.jvalue = phoenix::arg1 ] >> *boost::spirit::space_p;

                json_r =
                        atom[ json_r.jvalue = phoenix::arg1 ]
                        | object[ json_r.jvalue = phoenix::arg1 ]
                        | array[ json_r.jvalue = phoenix::arg1 ];

                object =
                        boost::spirit::chlit< wchar_t >( L'{' )[ object.jvalue = json::object_t() ] >> *boost::spirit::space_p
                        >> !(
                            ( json_string_p[ object.key = phoenix::arg1 ]
                            >> *boost::spirit::space_p >> boost::spirit::chlit< wchar_t >( L':' ) >> *boost::spirit::space_p
                            >> json_r[ object.value = phoenix::arg1 ] )[ detail::insert( object.jvalue, object.key, object.value ) ]
                            >> *( *boost::spirit::space_p >> boost::spirit::chlit< wchar_t >( L',' ) >> *boost::spirit::space_p
                                >> (
                                    json_string_p[ object.key = phoenix::arg1 ]
                                    >> *boost::spirit::space_p >> boost::spirit::chlit< wchar_t >( L':' ) >> *boost::spirit::space_p
                                    >> json_r[ object.value = phoenix::arg1 ]
                                )[ detail::insert( object.jvalue, object.key, object.value ) ]
                            )
                        ) >> *boost::spirit::space_p >> boost::spirit::chlit< wchar_t >( L'}' );

                array =
                        boost::spirit::chlit< wchar_t >( L'[' )[ array.jvalue = fostlib::json::array_t() ] >> *boost::spirit::space_p
                        >> !(
                            json_r[ detail::push_back( array.jvalue, phoenix::arg1 ) ]
                            >> *(
                                *boost::spirit::space_p >> boost::spirit::chlit< wchar_t >( L',' ) >> *boost::spirit::space_p
                                >> json_r[ detail::push_back( array.jvalue, phoenix::arg1 ) ]
                            )
                        ) >> *boost::spirit::space_p >> boost::spirit::chlit< wchar_t >( L']' );

                atom =
                            boost::spirit::strlit< wliteral >( L"null" )
                            | boolean[ atom.jvalue = phoenix::arg1 ]
                            | number[ atom.jvalue = phoenix::arg1 ]
                            | json_string_p[ atom.jvalue = phoenix::arg1 ];

                number =
                            boost::spirit::longest_d[
                                boost::spirit::int_parser< int64_t >()[ number.jvalue = phoenix::arg1 ]
                                | boost::spirit::strict_real_p[ number.jvalue = phoenix::arg1 ]
                            ];

                boolean =
                            boost::spirit::strlit< wliteral >( L"true" )[ boolean.jvalue = true ]
                            | boost::spirit::strlit< wliteral >( L"false" )[ boolean.jvalue = false ];
            }
            boost::spirit::rule< scanner_t, json_closure::context_t >
                    json_r, object, array, atom, number, boolean, null;
            boost::spirit::rule< scanner_t > top;

            boost::spirit::rule< scanner_t > const &start() const { return top; }
       };
    } json_p;


}


#endif // FOST_PARSE_JSON_HPP
