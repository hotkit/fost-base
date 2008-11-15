/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_URL_HPP
#define FOST_PARSE_URL_HPP


#include <fost/json.hpp>
#include <fost/parse/parse.hpp>


namespace fostlib {


    namespace detail {


        struct query_string_closure : boost::spirit::closure< query_string_closure, fostlib::string, std::vector< wchar_t >, wchar_t > {
            member1 text;
            member2 buffer;
            member3 character;
        };


    }


    extern const struct query_string_parser : public boost::spirit::grammar< query_string_parser, detail::query_string_closure::context_t > {
        template< typename scanner_t >
        struct definition {
            definition( query_string_parser const& self ) {
                top = !boost::spirit::list_p( pair, boost::spirit::chlit< wchar_t >( '&' ) );

                pair = key >> boost::spirit::chlit< wchar_t >( '=' ) >> !value;

                key = +boost::spirit::chset<>( L"a-zA-Z0-9." );

                value = +boost::spirit::chset<>( L"a-zA-Z0-9.,%" );
            }
            boost::spirit::rule< scanner_t > top, pair, key, value;

            boost::spirit::rule< scanner_t > const &start() const { return top; }
        };
    } query_string_p;


}


#endif // FOST_PARSE_URL_HPP
