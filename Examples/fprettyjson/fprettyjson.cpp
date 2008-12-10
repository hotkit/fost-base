/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main.hpp>
#include <fost/unicode.hpp>


using namespace fostlib;


FSL_MAIN(
    L"fprettyjson",
    L"JSON pretty printer\nCopyright (c) 2008, Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    if ( args[ 1 ].isnull() ) {
        out << L"No JSON file to load was specified" << std::endl;
        return 1;
    }
    json blob( json::parse( utf::load_file( coerce< utf8string >( args[ 1 ].value() ).c_str() ) ) );
    if ( args[2].isnull() )
        out << json::unparse( blob, true ) << std::endl;
    else {
        utf::save_file( coerce< utf8string >( args[ 2 ].value() ).c_str(), json::unparse( blob, true ) );
        out << L"JSON saved to " << args[ 2 ].value() << std::endl;
    }
    return 0;
}
