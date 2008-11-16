/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_HOST_HPP
#define FOST_PARSE_HOST_HPP


#include <fost/host.hpp>
#include <fost/parse/parse.hpp>


namespace fostlib {


    namespace detail {


        struct host_closure : boost::spirit::closure< host_closure,
            host,
            uint32_t
        > {
            member1 host;
            member2 ipv4;
        };


    }


    extern const struct host_parser : public boost::spirit::grammar <
        host_parser, detail::host_closure::context_t
    > {
        template< typename scanner_t >
        struct definition {
            definition( host_parser const &self ) {
                top = hostname | rawipv4 | ipv4address;
            }
            boost::spirit::rule< scanner_t > top, rawipv4, ipv4address;
            boost::spirit::rule< scanner_t, utf8_string_builder_closure::context_t > hostname;

            boost::spirit::rule< scanner_t > const &start() const { return top; }
        };
    } host_p;


}


#endif // FOST_PARSE_HOST_HPP
