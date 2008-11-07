/*
    Copyright 1995-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_ARGUMENTS_HPP
#define FOST_ARGUMENTS_HPP
#pragma once


#include <fost/string.hpp>
#include <fost/nullable.hpp>


namespace fostlib {


    class FOST_CLI_DECLSPEC arguments {
    public:
        typedef std::vector< fostlib::string >::size_type size_type;

        arguments( int argc, native_char *argv[] );
        arguments( int argc, native_char *argv[], native_char *envp[] );

        void environment( const string &envName, const string &section, const string &name );
        void argument( size_type argument, const string &section, const string &name );
        nullable< string > commandSwitch( const string &theSwitch ) const;
        void commandSwitch( const string &theSwitch, const string &section, const string &name );

        size_type size() const;
        nullable< string  > operator[]( size_type pos ) const;

        ostream &printOn( ostream &o ) const;

    private:
        void load( int argc, native_char *argv[] );
        void load( native_char *envp[] );

        std::map< string, string  > m_environment;
        std::vector< string > m_arguments;
        std::map< string, string > m_switches;
        std::list< boost::shared_ptr< setting< json > > > m_registered;
    };


}


#endif // FOST_ARGUMENTS_HPP

