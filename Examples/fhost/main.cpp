/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/inet>
#include <fost/cli>
#include <fost/main.hpp>


using namespace fostlib;


FSL_MAIN(
    L"fhost",
    L"fhost\nCopyright (c) 2008 Felspar Co. Ltd."
)( ostream &out, arguments &args ) {
    for ( arguments::size_type c( 1 ); c < args.size(); ++c )
        try {
            out <<
                args[ c ].value() << L" -> " <<
                coerce< string >( host( args[ c ].value() ).address() ) << std::endl;
        } catch ( exceptions::host_not_found & ) {
            out << args[ c ].value() << L" -- Address not found" << std::endl;
        }
    return 0;
}
