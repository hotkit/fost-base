/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_PARSE_HPP
#define FOST_PARSE_PARSE_HPP


#ifdef WIN32
    #pragma warning ( disable : 4709 ) // comma operator within array index expression
    #pragma warning ( disable : 4244 ) //conversion from 'int' to 'FSLib::utf16', possible loss of data
#endif
#include <boost/spirit.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/functions.hpp>


namespace fostlib {


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
        };
        phoenix::function<insert_impl> const insert = insert_impl();


    }


}


#endif // FOST_PARSE_PARSE_HPP
