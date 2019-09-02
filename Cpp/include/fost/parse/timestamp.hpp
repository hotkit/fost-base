/**
    Copyright 2017-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/parse/parse.hpp>

#include <boost/spirit/include/phoenix.hpp>


namespace fostlib {


    template<typename Iterator>
    struct rfc1123_timestamp_parser :
    public boost::spirit::qi::
            grammar<Iterator, timestamp(), decltype(boost::spirit::qi::space)> {
        boost::spirit::qi::
                rule<Iterator, timestamp(), decltype(boost::spirit::qi::space)>
                        top;
        boost::spirit::qi::symbols<char, unsigned> weekday, month;

        rfc1123_timestamp_parser() : rfc1123_timestamp_parser::base_type(top) {
            using boost::spirit::qi::_2;
            using boost::spirit::qi::_3;
            using boost::spirit::qi::_4;
            using boost::spirit::qi::_5;
            using boost::spirit::qi::_6;
            using boost::spirit::qi::_7;
            using boost::spirit::qi::_val;

            top = (weekday // _1
                   >> boost::spirit::qi::uint_ // _2
                   >> month // _3
                   >> boost::spirit::qi::uint_ // _4
                   >> boost::spirit::qi::uint_
                   >> boost::spirit::qi::lit(':') // _5
                   >> boost::spirit::qi::uint_
                   >> boost::spirit::qi::lit(':') // _6
                   >> boost::spirit::qi::uint_ // _7
                   >> boost::spirit::qi::string("+0000"))[boost::phoenix::bind(
                    [](auto &v, auto year, auto month, auto date, auto hour,
                       auto minute, auto second) {
                        v = timestamp(year, month, date, hour, minute, second);
                    },
                    _val, _4, _3, _2, _5, _6, _7)];

            weekday.add("Mon,", 1)("Tue,", 2)("Wed,", 3)("Thu,", 4)("Fri,", 5)(
                    "Sat,", 6)("Sun,", 7);

            month.add("Jan", 1)("Feb", 2)("Mar", 3)("Apr", 4)("May", 5)(
                    "Jun", 6)("Jul", 7)("Aug", 8)("Sep", 9)("Oct", 10)(
                    "Nov", 11)("Dec", 12);
        }
    };


}
