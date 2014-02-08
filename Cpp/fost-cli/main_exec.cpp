/*
    Copyright 2008-2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-cli.hpp"
#include <fost/main.hpp>

#include <boost/bind.hpp>


fostlib::ini_settings::ini_settings( const fostlib::string &name, const fostlib::string &banner )
: name( name ), banner( banner ),
    c_iniFile( L"fost-cli/main.cpp", name, L"IniFile", name + L".ini", true ),
    c_banner( L"fost-cli/main.cpp", name, L"Banner", true, true ),
    c_settings( L"fost-cli/main.cpp", name, L"Settings", false, true ),
    c_environment( L"fost-cli/main.cpp", name, L"Environment", false, true ),
    c_logging( L"fost-cli/main.cpp", name, "Logging sinks", fostlib::json::parse("{"
            "\"sinks\":[{"
                "\"name\": \"stdout\", \"configuration\": {}"
            "}]"
        "}"), true ) {
}


void fostlib::standard_arguments(
    const ini_settings &settings,
    ostream &out,
    arguments &args
) {
    args.commandSwitch( L"b", settings.name, L"Banner" );
    if ( settings.c_banner.value() )
        out << settings.banner << std::endl;

    args.commandSwitch( L"s", settings.name, L"Settings" );
    if ( settings.c_settings.value() )
        setting< json >::printAllOn( out );

    args.commandSwitch( L"e", settings.name, L"Environment" );
    if ( settings.c_environment.value() )
        args.printOn( out );
}


namespace {
    int exception_wrapper( fostlib::ostream &out, boost::function< int () > f ) {
        try {
            fostlib::exceptions::structured_handler handler;
#ifdef WIN32
            fostlib::ini_file base_settings_file(fostlib::arguments::environment("windir") + L"\\fost.ini");
#else
            fostlib::ini_file base_settings_file(L"/etc/fost.conf");
#endif

            return f();
        } catch ( fostlib::exceptions::exception &e ) {
            out << std::endl << L"Caught a fostlib::exceptions::exception" << std::endl;
            out << e << std::endl;
        } catch ( std::exception &e ) {
            out << std::endl << L"Caught a std::exception" << std::endl;
            out << fostlib::string( e.what() ) << std::endl;
        } catch ( ... ) {
            out << std::endl << L"Unknown exception type caught" << std::endl;
        }
        return 2;
    }

    int simple_wrapper(
        const fostlib::ini_settings &settings,
        fostlib::ostream &out,
        fostlib::arguments &args,
        int (*main_f)( fostlib::ostream &, fostlib::arguments & )
    ) {
        args.commandSwitch( L"i", settings.name, L"IniFile" );
        fostlib::ini_file ifile( settings.c_iniFile.value() );

        fostlib::standard_arguments( settings, out, args );
        return main_f( out, args );
    }
    int complex_wrapper(
        const fostlib::ini_settings &settings,
        fostlib::ostream &out,
        fostlib::arguments &args,
        int (*main_f)( const fostlib::ini_settings &, fostlib::ostream &, fostlib::arguments & )
    ) {
        return main_f( settings, out, args );
    }
}


int fostlib::main_exec(
    const ini_settings &settings,
    ostream &out,
    arguments &args,
    int (*main_f)( fostlib::ostream &, fostlib::arguments & )
) {
    return exception_wrapper( out, boost::bind( simple_wrapper, boost::cref( settings ), boost::ref( out ), boost::ref( args ), main_f ) );
}

int fostlib::main_exec(
    const ini_settings &settings,
    ostream &out,
    arguments &args,
    int (*main_f)( const fostlib::ini_settings &, fostlib::ostream &, fostlib::arguments & )
) {
    return exception_wrapper( out, boost::bind( complex_wrapper, boost::cref( settings ), boost::ref( out ), boost::ref( args ), main_f ) );
}
