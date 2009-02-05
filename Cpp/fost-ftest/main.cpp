/*
    Copyright 2005-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifdef WIN32
    #define FOST_CORE_DECLSPEC __declspec( dllimport )
    #define FOST_CLI_DECLSPEC __declspec( dllimport )
    #define FOST_TEST_DECLSPEC __declspec( dllimport )
#else
    #define FOST_CORE_DECLSPEC
    #define FOST_CLI_DECLSPEC
    #define FOST_TEST_DECLSPEC
#endif

#include <fost/exception.hpp>
#include <fost/coerce.hpp>
#include <fost/string/coerce.hpp>
#include <fost/settings.hpp>
#include <fost/string/utility-nullable.hpp>
#include <fost/dynlib.hpp>

#include <fost/main.hpp>
#include <fost/test.hpp>


namespace {
    const fostlib::setting< bool > c_verbose( L"", L"FTest", L"Verbose", false, true );
}


FSL_MAIN(
    L"ftest",
    L"ftest - FOST.3 Test executor\nCopyright (c) 1995-2008, Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    args.commandSwitch( L"v", c_verbose.section(), c_verbose.name() );
    if ( args.size() < 2 )
        throw fostlib::exceptions::null( L"Must specify at least one DLL to load tests for" );
    else {
        std::list< boost::shared_ptr< fostlib::dynlib > > libraries;
        for ( fostlib::arguments::size_type i( 1 ); i < args.size(); ++i )
            libraries.push_back( boost::shared_ptr< fostlib::dynlib >( new fostlib::dynlib( args[ i ].value() ) ) );
        if ( c_verbose.value() ? fostlib::test::suite::execute( out ) : fostlib::test::suite::execute() )
            return 1;
        else
            return 0;
    }
}

