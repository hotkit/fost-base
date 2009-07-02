/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_PARSE_HPP
#define FOST_PARSE_PARSE_HPP


// Used for configuring Boost 1.38.0
#define BOOST_SPIRIT_USE_OLD_NAMESPACE


#ifdef WIN32
    #pragma warning ( disable : 4709 ) // comma operator within array index expression
    #pragma warning ( disable : 4244 ) //conversion from 'int' to 'FSLib::utf16', possible loss of data
#endif
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/phoenix1.hpp>

#include <fost/string/tagged-string.hpp>


namespace fostlib {


        struct utf16_string_builder_closure : boost::spirit::closure< utf16_string_builder_closure,
            fostlib::string, std::vector< wchar_t >, wchar_t
        > {
            member1 text;
            member2 buffer;
            member3 character;
        };
        struct utf8_string_builder_closure : boost::spirit::closure< utf8_string_builder_closure,
            fostlib::string, std::vector< utf8 >, utf8
        > {
            member1 text;
            member2 buffer;
            member3 character;
        };
        struct ascii_string_builder_closure : boost::spirit::closure< ascii_string_builder_closure,
            fostlib::ascii_string, std::vector< char >, char
        > {
            member1 text;
            member2 buffer;
            member3 character;
        };


    namespace detail {


        // Implementation taken from
        // http://spirit.sourceforge.net/distrib/spirit_1_8_5/libs/spirit/example/fundamental/stuff_vector.cpp
        struct push_back_impl {
            template <typename Container, typename Item>
            struct result {
                typedef void type;
            };
            template <typename Container, typename Item>
            void operator()(Container& c, Item const& item) const {
                c.push_back(item);
            }
            void operator()(json& c, json const& item) const {
                jcursor().push_back( c, item );
            }
        };
        phoenix::function<push_back_impl> const push_back = push_back_impl();

        struct insert_impl {
            template <typename Container, typename Key, typename Value>
            struct result {
                typedef void type;
            };
            template <typename Container, typename Key, typename Value>
            void operator()(Container& c, Key const& key, Value const& value) const {
                c.insert( key, value );
            }
            void operator()( json &c, string const& key, json const& value) const {
                (jcursor( key ))( c ) = value;
            }
        };
        phoenix::function<insert_impl> const insert = insert_impl();


    }


}


#endif // FOST_PARSE_PARSE_HPP
