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


namespace {
    setting< bool > c_overwrite( L"fprettyjson/fprettyjson.cpp", L"fprettyjson", L"Overwrite original", false, true );
}


FSL_MAIN(
    L"fprettyjson",
    L"JSON pretty printer\nCopyright (c) 2008, Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    /*
        Process the extra command line parameters we want
    */
    args.commandSwitch( L"o", L"fprettyjson", L"Overwrite original" );
    args.commandSwitch( L"s", L"JSON", L"Unparse tab width" );
    /*
        Check that we have an input file name
    */
    if ( args[ 1 ].isnull() ) {
        out << L"No JSON file to load was specified" << std::endl;
        return 1;
    }
    /*
        Load the JSON object
    */
    json blob( json::parse( utf::load_file( coerce< utf8string >( args[ 1 ].value() ).c_str() ) ) );
    /*
        If we don't have a 2nd filename and we're not overwriting then output to the screen
    */
    if ( args[2].isnull() && !c_overwrite.value() )
        out << json::unparse( blob, true ) << std::endl;
    else {
        /*
            Otherwise output to the 2nd argument or back to the 1st
        */
        string ofile( args[ 2 ].value( args[ 1 ].value() ) );
        utf::save_file( coerce< utf8string >( ofile ).c_str(), json::unparse( blob, true ) );
        out << L"JSON saved to " << ofile << std::endl;
    }
    return 0;
}
