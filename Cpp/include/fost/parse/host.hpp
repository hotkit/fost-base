/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_PARSE_HOST_HPP
#define FOST_PARSE_HOST_HPP


#include <fost/internet/host.hpp>
#include <fost/parse/parse.hpp>


namespace fostlib {


    namespace detail {


        struct host_closure : boost::spirit::closure< host_closure,
            host,
            string,
            uint32_t
        > {
            member1 host;
            member2 hostname;
            member3 ipv4;
        };


    }


    extern const FOST_INET_DECLSPEC struct host_parser : public boost::spirit::grammar <
        host_parser, detail::host_closure::context_t
    > {
        template< typename scanner_t >
        struct definition {
            definition( host_parser const &self ) {
                top =
                    ipv4address[ self.host = phoenix::construct_< fostlib::host >( self.ipv4 ) ]
                    | fqname[ self.host = phoenix::construct_< fostlib::host >( self.hostname ) ]
                    | rawipv4[ self.host = phoenix::construct_< fostlib::host >( self.ipv4 ) ]
                    | hname[ self.host = phoenix::construct_< fostlib::host >( self.hostname ) ];

                rawipv4 = boost::spirit::uint_parser< uint32_t, 10, 1, 10 >()[
                    self.ipv4 = phoenix::arg1
                ];

                ipv4address =
                    boost::spirit::uint_parser< uint8_t, 10, 1, 3 >()[ self.ipv4 = phoenix::arg1 ]
                    >> boost::spirit::chlit< wchar_t >( '.' )[ self.ipv4 *= 0x100 ]
                    >> boost::spirit::uint_parser< uint8_t, 10, 1, 3 >()[ self.ipv4 += phoenix::arg1 ]
                    >> boost::spirit::chlit< wchar_t >( '.' )[ self.ipv4 *= 0x100 ]
                    >> boost::spirit::uint_parser< uint8_t, 10, 1, 3 >()[ self.ipv4 += phoenix::arg1 ]
                    >> boost::spirit::chlit< wchar_t >( '.' )[ self.ipv4 *= 0x100 ]
                    >> boost::spirit::uint_parser< uint8_t, 10, 1, 3 >()[ self.ipv4 += phoenix::arg1 ];

                hname = boost::spirit::chset<>( L"a-zA-Z0-9" )[
                    self.hostname = phoenix::construct_< fostlib::string >( 1, phoenix::arg1 )
                ] >> *boost::spirit::chset<>( L"a-zA-Z0-9-" )[
                    self.hostname += phoenix::arg1
                ];
                dname = +boost::spirit::chset<>( L"a-zA-Z0-9-" )[
                    self.hostname += phoenix::arg1
                ];
                fqname = hname >> +(
                    boost::spirit::chlit< wchar_t >( '.' )[
                        self.hostname += phoenix::arg1
                    ] >> dname
                );
            }
            boost::spirit::rule< scanner_t > top, fqname, dname, hname, rawipv4, ipv4address;

            boost::spirit::rule< scanner_t > const &start() const { return top; }
        };
    } host_p;


}


#endif // FOST_PARSE_HOST_HPP
