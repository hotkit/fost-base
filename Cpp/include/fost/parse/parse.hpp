/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_PARSE_HPP
#define FOST_PARSE_PARSE_HPP
#pragma once


// This may make the grammars safer in a multi-threaded environment
#define BOOST_SPIRIT_THREADSAFE

#define BOOST_RESULT_OF_USE_DECLTYPE
#define BOOST_SPIRIT_USE_PHOENIX_V3

#include <boost/spirit/include/qi_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>

#include <fost/tagged-string.hpp>


namespace fostlib {


    namespace detail {


        /// Returns a mutex used to serialise access to the Boost Spirit parsers
        FOST_CORE_DECLSPEC
        boost::recursive_mutex &g_parser_mutex();


    }


    /// RAII wrapper for the parser lock to serialise parses
    class parser_lock {
        boost::recursive_mutex::scoped_lock lock;
    public:
        parser_lock()
        : lock(detail::g_parser_mutex()) {
        }
    };


    /// Hex escaped char, as used in URLs etc.
    template<typename Iterator>
    struct hex_char : public boost::spirit::qi::grammar<Iterator, uint8_t> {
        boost::spirit::qi::rule<Iterator, uint8_t> top;

        hex_char()
        : hex_char::base_type(top) {
            top = boost::spirit::qi::lit('%') >> boost::spirit::qi::uint_parser<uint8_t, 16, 2, 2>();
        }
    };


}


#endif // FOST_PARSE_PARSE_HPP
