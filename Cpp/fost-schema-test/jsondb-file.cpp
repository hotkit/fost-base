/*
    Copyright 2005-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-schema-test.hpp"
#include <fost/core>
#include <fost/cli>
#include <fost/main.hpp>
#include <fost/unicode.hpp>

#include <fost/db.hpp>


using namespace fostlib;


FSL_MAIN(
    L"fost-schema-test-jsondb-file",
    L"fost-schema-test-jsondb-file\nTest the file handling for Fost 4 JSON databases\nCopyright (c) 2008, Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    if ( args[ 1 ].isnull() ) {
        out << L"Supply a JSON file which contains the database configuration as the first parameter" << std::endl;
        return 1;
    }

    json configuration( json::parse( utf::load_file( coerce< utf8string >( args[ 1 ].value() ).c_str() ) ) );
    if ( !configuration.has_key( L"root" ) )
        jcursor()[ L"root" ]( configuration ) = L"Cpp/fost-schema-test/jsondb-file";
    dbconnection dbc( configuration );

    return 0;
}

