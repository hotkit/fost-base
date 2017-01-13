/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_JSON_HPP
#define FOST_PARSE_JSON_HPP
#pragma once


#include <fost/json.hpp>
#include <fost/unicode.hpp>
#include <fost/parse/parse.hpp>

#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/fusion/include/std_pair.hpp>


namespace fostlib {


    template<typename Iterator>
    struct json_string_parser : public boost::spirit::qi::grammar<Iterator, string()> {
        static_assert(sizeof(decltype(*Iterator())) == 2,
            "The JSON parrsing iterator must produce UTF-16");

        boost::spirit::qi::rule<Iterator, string()> top;
        boost::spirit::qi::rule<Iterator, std::vector<f5::utf16>()> str;
        boost::spirit::qi::rule<Iterator, f5::utf16> escaped;

        json_string_parser()
        : json_string_parser::base_type(top) {
            using boost::spirit::qi::lit;
            using boost::spirit::qi::_1;
            using boost::spirit::qi::_val;

            top = str[boost::phoenix::bind([](auto &v, auto &s) {
                    auto pos = f5::make_u16u32_iterator<exceptions::unicode_encoding>(s.begin(), s.end());
                    for ( ; pos.first != pos.second; ++pos.first ) {
                        v += *pos.first;
                    }
                }, _val, _1)];

            str = lit('"') >> *((lit('\\') > escaped) | (boost::spirit::qi::standard_wide::char_ - '"')) >> lit('"');

            escaped = boost::spirit::qi::char_('"')[_val = '"']
                | boost::spirit::qi::char_('\\')[_val = '\\']
                | boost::spirit::qi::char_('/')[_val = '/']
                | boost::spirit::qi::char_('b')[_val = 0x08]
                | boost::spirit::qi::char_('f')[_val = 0x0c]
                | boost::spirit::qi::char_('n')[_val = '\n']
                | boost::spirit::qi::char_('r')[_val = '\r']
                | boost::spirit::qi::char_('t')[_val = '\t']
                | (lit('u') >> boost::spirit::qi::uint_parser<f5::utf16, 16, 4, 4>())[_val = _1];
        }
    };


    template<typename Iterator>
    struct json_embedded_parser : public boost::spirit::qi::grammar<Iterator, json()> {
        using object_pair_t = std::pair<string, json>;

        boost::spirit::qi::rule<Iterator, json()> top, atom, null, boolean, number;
        boost::spirit::qi::rule<Iterator, object_pair_t()> object_pair;
        boost::spirit::qi::rule<Iterator, json::object_t()> object, object_array;
        boost::spirit::qi::rule<Iterator, json::array_t()> array, array_list;
        boost::spirit::qi::real_parser<double, boost::spirit::qi::strict_real_policies<double>> real_p;
        json_string_parser<Iterator> json_string_p;
        boost::spirit::qi::rule<Iterator, void()> whitespace;

        json_embedded_parser()
        : json_embedded_parser::base_type(top) {
            using boost::spirit::qi::_1;
            using boost::spirit::qi::_val;

            /// A non-capture whitespace parser
            whitespace = *(boost::spirit::qi::lit(' ') | '\n' | '\t' | '\r');

            top = object | array | atom;

            object = (boost::spirit::qi::lit('{') >> whitespace
                    >> -object_array >> whitespace >> boost::spirit::qi::lit('}'));
            object_pair = (json_string_p >> whitespace >> boost::spirit::qi::lit(':') >> whitespace >> top);
            object_array = object_pair % ( whitespace >> boost::spirit::qi::lit(',') >> whitespace);

            array = (boost::spirit::qi::lit('[') >> whitespace
                    >> -array_list >> whitespace >> boost::spirit::qi::lit(']'));
            array_list = top % (whitespace >> boost::spirit::qi::lit(',') >> whitespace);

            null = boost::spirit::qi::string("null")[_val = json()];
            boolean = boost::spirit::qi::string("false")[_val = json(false)]
                | boost::spirit::qi::string("true")[_val = json(true)];
            number = real_p[_val = _1]
                | boost::spirit::qi::int_parser<int64_t>()[_val = _1];

            atom = null | boolean | number | json_string_p;
        }
    };


    template<typename Iterator>
    struct json_parser : boost::spirit::qi::grammar<Iterator, json()> {
        boost::spirit::qi::rule<Iterator, json()> top;
        json_embedded_parser<Iterator> embedded;

        json_parser()
        : json_parser::base_type(top) {
            using boost::spirit::qi::_2;
            using boost::spirit::qi::_val;

            top = (*boost::spirit::qi::space >> embedded >> *boost::spirit::qi::space)
                [_val = _2];
        }
    };


}


#endif // FOST_PARSE_JSON_HPP
