/**
    Copyright 1995-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_ARGUMENTS_HPP
#define FOST_ARGUMENTS_HPP
#pragma once


#include <fost/string.hpp>
#include <fost/nullable.hpp>


namespace fostlib {


    /// Command line argument handling
    class FOST_CLI_DECLSPEC arguments {
    public:
        /// Size type for the argument list
        typedef std::vector< fostlib::string >::size_type size_type;

        /// Construct from the parameters to main (or wmain)
        arguments( int argc, const native_char * const argv[] );
        /// Construct with an environment pointer
        arguments(
            int argc, const native_char * const argv[], const native_char * const envp[]
        );

        static string environment( const string &env_name );
        void environment(
            const string &env_name, const string &section, const string &name
        );

        void argument( size_type argument, const string &section, const string &name );

        nullable< string > commandSwitch( const string &theSwitch ) const;
        void commandSwitch(
            const string &theSwitch, const string &section, const string &name
        );
        void commandSwitch(
            const string &theSwitch, const setting<json> &setting);

        /// Return the number of arguments
        size_type size() const {
            return m_arguments.size();
        }
        nullable< string  > operator[]( size_type pos ) const;

        /// Allow iteration over the arguments
        using const_iterator = typename std::vector<string>::const_iterator;
        const_iterator begin() const {
            /// The first argument is always the program name, so any program
            /// that wishes to iterate over the arguments actually passed in
            /// should skip this argument. It can always be accessed through
            /// the zeroth index.
            auto it = m_arguments.begin();
            if ( it != m_arguments.end() ) {
                return ++it;
            } else {
                return it;
            }
        }
        const_iterator end() const {
            return m_arguments.end();
        }

        ostream &printOn( ostream &o ) const;

        const std::map< string, string > &switches() const {
            return m_switches;
        }

    private:
        void load( int argc, const native_char * const argv[] );
        void load( const native_char * const envp[] );

        std::map< string, string  > m_environment;
        std::vector< string > m_arguments;
        std::map< string, string > m_switches;
        std::list< boost::shared_ptr< setting< json > > > m_registered;
    };


}


#endif // FOST_ARGUMENTS_HPP

