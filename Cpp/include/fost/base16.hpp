/*
    Copyright 2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <fost/string>
#include <fost/exception/parse_error.hpp>


namespace fostlib {


    /// Base16 string definition
    struct base16_string_tag {
        static void do_encode(nliteral l, ascii_string &s);
        static void do_encode(const ascii_string &l, ascii_string &s);
        static void check_encoded(const ascii_string &s);
    };

    /// A base16 string
    using base16_string = tagged_string<base16_string_tag, ascii_string>;


    /// Convert base16 into a byte vector. Throw a parse error if there
    /// isn't an odd number of characters
    template<>
    struct coercer<std::vector<unsigned char>, base16_string> {
        std::vector<unsigned char> coerce(const base16_string &s);
    };


}
