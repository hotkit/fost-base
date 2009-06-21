/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_MAIN_HPP
#define FOST_MAIN_HPP
#pragma once


#include <fost/settings.hpp>
#include <fost/arguments.hpp>
#include <fost/inifile.hpp>
#include <iostream>


namespace fostlib {


    struct FOST_CLI_DECLSPEC ini_settings {
        ini_settings( const string &name, const string &banner );
        const string name;
        const string banner;
        const fostlib::setting< fostlib::string > c_iniFile;
        const fostlib::setting< bool > c_banner;
        const fostlib::setting< bool > c_settings;
        const fostlib::setting< bool > c_environment;
    };

    FOST_CLI_DECLSPEC void standard_arguments(
        const fostlib::ini_settings &settings,
        ostream &out,
        arguments &args
    );

    // Used by FSL_MAIN
    FOST_CLI_DECLSPEC int main_exec(
        const ini_settings &settings,
        ostream &out,
        arguments &args, int (*main_f)( fostlib::ostream &, fostlib::arguments & )
    );
    // Used by FSL_MAIN_INTERPRETER
    FOST_CLI_DECLSPEC int main_exec(
        const ini_settings &settings,
        ostream &out,
        arguments &args, int (*main_f)( const ini_settings &, fostlib::ostream &, fostlib::arguments & )
    );


}


#define FSL_MAIN_PRIVATE_COMMON( exe_name, banner_text ) \
        const fostlib::ini_settings config_settings( fostlib::string( exe_name ), fostlib::string( banner_text ) );

#define FSL_MAIN_PRIVATE_SIMPLEMAIN() \
        int main_body( fostlib::ostream &, fostlib::arguments & );
#define FSL_MAIN_PRIVATE_INTERPRETERMAIN() \
        int main_body( const fostlib::ini_settings &, fostlib::ostream &, fostlib::arguments & );

#ifdef WIN32

    #define FSL_MAIN_PRIVATE_CALLER( exe_name, banner_text ) \
        int __cdecl wmain( int argc, wchar_t *argv[ ], wchar_t *envp[ ] ) { \
            fostlib::com_hr( ::CoInitializeEx( NULL, COINIT_APARTMENTTHREADED ), L"CoInitializeEx at program start" ); \
            fostlib::arguments args( argc, argv, envp ); \
            return fostlib::main_exec( config_settings, std::wcout, args, main_body ); \
        } \
        int main_body

#else

    #define FSL_MAIN_PRIVATE_CALLER( exe_name, banner_text ) \
        int main( int argc, char *argv[ ] ) { \
            fostlib::arguments args( argc, argv ); \
            return fostlib::main_exec( config_settings, std::cout, args, main_body ); \
        } \
        int main_body

#endif

/*
    This version of main is suitable for most programs that simply need to take settings
    and start some task for the user.
    This version will load the INI file for the program and the -i command line switch is handled.
*/
#define FSL_MAIN( exe_name, banner_text ) \
        FSL_MAIN_PRIVATE_COMMON( exe_name, banner_text ) \
        FSL_MAIN_PRIVATE_SIMPLEMAIN() \
        FSL_MAIN_PRIVATE_CALLER( exe_name, banner_text )

/*
    This version of main is suitable for programs that act as interpreters for other languages.
    The standard_arguments must be called explicitly, but the interpreter has the opportunity
    to layer in any extra configuration settings that it may want to do before the standard
    argument handling is done. The version of main written for this will also be passed the
    settings object.
    NB: The INI file for the program is not automatically loaded by this version. The -i command
    line switch is not handled either.
*/
#define FSL_MAIN_INTERPRETER( exe_name, banner_text ) \
        FSL_MAIN_PRIVATE_COMMON( exe_name, banner_text ) \
        FSL_MAIN_PRIVATE_INTERPRETERMAIN() \
        FSL_MAIN_PRIVATE_CALLER( exe_name, banner_text )


#endif // FOST_MAIN_HPP
