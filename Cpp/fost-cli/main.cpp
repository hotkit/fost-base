/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/main.hpp>


fostlib::ini_settings::ini_settings( const fostlib::string &name, const fostlib::string &banner )
: name( name ), banner( banner ),
    c_iniFile( L"", name, L"IniFile", name + L".ini", true ),
    c_banner( L"", name, L"Banner", true, true ),
    c_settings( L"", name, L"Settings", false, true ),
    c_environment( L"", name, L"Environment", false, true ) {
}


int fostlib::main_exec( const fostlib::ini_settings &settings, ostream &out, arguments &args, int (*main_f)( fostlib::ostream &, fostlib::arguments & ) ) {
    fostlib::exceptions::structured_handler handler;
    try {
        args.commandSwitch( L"i", settings.name, L"IniFile" );
        fostlib::ini_file ifile( settings.c_iniFile.value() );

        args.commandSwitch( L"b", settings.name, L"Banner" );
        if ( settings.c_banner.value() )
            out << settings.banner << std::endl;

        args.commandSwitch( L"s", settings.name, L"Settings" );
        if ( settings.c_settings.value() )
            setting< json >::printAllOn( out );

        args.commandSwitch( L"s", settings.name, L"Environment" );
        if ( settings.c_environment.value() )
            args.printOn( out );

        return main_f( out, args );
    } catch ( fostlib::exceptions::exception &e ) {
        out << std::endl << L"Caught a fostlib::exceptions::exception" << std::endl << e << std::endl;
    } catch ( std::exception &e ) {
        out << std::endl << L"Caught a std::exception" << std::endl << fostlib::string( e.what() ) << std::endl;
    } catch ( ... ) {
        out << std::endl << L"Unknown exception type caught" << std::endl;
    }
    return 2;
}

