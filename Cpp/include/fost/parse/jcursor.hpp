/**
    Copyright 2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <fost/parse/json.hpp>


namespace fostlib {


    template<typename Iterator>
    struct json_pointer_parser : public boost::spirit::qi::grammar<Iterator, jcursor()> {
        boost::spirit::qi::rule<Iterator, jcursor()> top;
        boost::spirit::qi::rule<Iterator, jcursor::stack_t()> parts;
        boost::spirit::qi::rule<Iterator, jcursor::index_t()> part;
        boost::spirit::qi::rule<Iterator, jcursor::number_t()> number_part;
        boost::spirit::qi::rule<Iterator, jcursor::string_t()> string_part;
        boost::spirit::qi::rule<Iterator, std::vector<f5::utf16>> characters;
        boost::spirit::qi::rule<Iterator, f5::utf16> tilde, slash;
        json_string_parser<Iterator> json_string;


        json_pointer_parser()
        : json_pointer_parser::base_type(top) {
            namespace qi = boost::spirit::qi;

            top = -parts;
            parts = *part;
            part = qi::lit('/') >> (number_part | string_part);
            number_part = qi::int_parser<jcursor::number_t>();
            string_part = characters[boost::phoenix::bind([](auto &v, auto &s) {
                auto pos = f5::make_u16u32_iterator<exceptions::unicode_encoding>(s.begin(), s.end());
                for ( ; pos.first != pos.second; ++pos.first ) {
                    v += *pos.first;
                }
            }, qi::_val, qi::_1)];
            characters = *(tilde | slash |
                (qi::lit('\\') > json_string.escaped) |
                qi::standard_wide::char_ - '/' - '~');
            tilde = qi::string("~0")[qi::_val = '~'];
            slash = qi::string("~1")[qi::_val = '/'];
        }
    };


    template<typename Iterator>
    struct json_pointer_fragment_parser :
        public boost::spirit::qi::grammar<Iterator, jcursor()>
    {
        static_assert(sizeof(decltype(*Iterator())) == 1,
            "The JSON pointer fragment parsing iterator must produce bytes");

        boost::spirit::qi::rule<Iterator, jcursor()> top;
        boost::spirit::qi::rule<Iterator, jcursor::stack_t()> parts;
        boost::spirit::qi::rule<Iterator, jcursor::index_t()> part;
        boost::spirit::qi::rule<Iterator, jcursor::string_t()> string_part;
        boost::spirit::qi::rule<Iterator, std::vector<unsigned char>()> characters;
        hex_char<Iterator> hex;
        boost::spirit::qi::rule<Iterator, f5::utf16> tilde, slash;
        boost::spirit::qi::rule<Iterator, jcursor::number_t()> number_part;

        json_pointer_fragment_parser()
        : json_pointer_fragment_parser::base_type(top) {
            namespace qi = boost::spirit::qi;

            top = qi::lit('#') >> parts;
            parts = *part;
            part = qi::lit('/') >> (number_part | string_part);
            number_part = qi::int_parser<jcursor::number_t>();
            string_part = characters[boost::phoenix::bind([](auto &v, auto &s) {
                v = fostlib::coerce<fostlib::string>(
                    fostlib::coerce<fostlib::utf8_string>(s));
            }, qi::_val, qi::_1)];
            characters = *(tilde | slash | hex |
                qi::standard_wide::char_ - '/' - '~' - '%');
            tilde = qi::string("~0")[qi::_val = '~'];
            slash = qi::string("~1")[qi::_val = '/'];
        }
    };


}

