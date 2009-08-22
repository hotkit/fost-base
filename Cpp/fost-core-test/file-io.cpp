/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main>
#include <fost/unicode>


using namespace fostlib;


FSL_MAIN(
    L"fost-core-test-file-io",
    L"fost-core-test-file-io\nTest file I/O and its Unicode handling\nCopyright (C) 2009, Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    // Check that we can do some basic reads
    { // Build a basic text stream that we want to check against
        std::ofstream outfile( coerce< utf8string >( args[ 1 ] ).c_str() );
        outfile.write( "abcdef\n", 7 );
        char tm[] = { 0xe2, 0x84, 0xa2, 0x00 };
        outfile.write( tm, 3 ); outfile.write( "\n", 1 );
    }
    string loaded = utf::load_file( coerce< std::wstring >( args[ 1 ] ) );
    if ( loaded != L"abcdef\n\x2122\n" ) {
        out << L"File loaded did not match file saved\n97 98 99 100 101 102 10 8482 10" << std::endl;
        for ( string::const_iterator i( loaded.begin() ); i != loaded.end(); ++i )
            out << int( *i ) << L" ";
        out << std::endl;
        return 1;
    }
    return 0;
}
