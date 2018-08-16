/**
    Copyright 2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#pragma once


#include <fost/parse/parse.hpp>


namespace fostlib {

    template<typename Iterator>
    struct jcursor_parser : public boost::spirit::qi::grammar<Iterator, jcursor()> {
        boost::spirit::qi::rule<Iterator, jcursor()> top;

        jcursor_parser()
        : jcursor_parser::base_type(top) {
        }
    };


    jcursor parse_jcursor_fragment(f5::u8view);


}

