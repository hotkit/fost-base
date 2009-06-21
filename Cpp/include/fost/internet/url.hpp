/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_URL_HPP
#define FOST_URL_HPP
#pragma once


#include <fost/internet/host.hpp>
#include <fost/string/tagged-string.hpp>


namespace fostlib {


    typedef uint16_t port_number;


    class FOST_INET_DECLSPEC url {
    public:
        struct FOST_INET_DECLSPEC filepath_string_tag {
            static void do_encode( fostlib::nliteral from, ascii_string &into );
            static void do_encode( const ascii_string &from, ascii_string &into );
            static void check_encoded( const ascii_string &s );
        };
        typedef tagged_string< filepath_string_tag, ascii_string > filepath_string;

        class FOST_INET_DECLSPEC query_string {
        public:
            query_string();
            query_string( const string & );

            void append( const string &name, const nullable< string > &value );
            void remove( const string &name );

            nullable< string > as_string() const;

        private:
            std::map< string, std::list< nullable< string > > > m_query;
        };
        enum t_form { e_pathname, e_encoded };

        url();
        explicit url( const string & );
        url( const url &base, const string &relative );
        url( const t_form, const string & );
        explicit url( const host &,
            const nullable< string > &username = null,
            const nullable< string > &password = null
        );
        url( const string &protocol, const host &,
            const nullable< string > &username = null,
            const nullable< string > &password = null
        );
        url( const string &protocol, const host &, port_number port,
            const nullable< string > &username = null,
            const nullable< string > &password = null
        );

        accessors< string > protocol;
        host server() const;
        accessors< nullable< port_number > > port;
        accessors< nullable< string > > user;
        accessors< nullable< string > > password;
        const string &pathspec() const;
        void pathspec( const string &pathName );
        accessors< nullable< string > > anchor;
        accessors< query_string, fostlib::lvalue > query;

        static setting< string > s_default_host;

        string as_string() const;
        string as_string( const url &relative_from ) const;

    private:
        fostlib::host m_host;
        string m_pathspec;
    };


    namespace exceptions {


        class FOST_INET_DECLSPEC relative_path_error : public exception {
        public:
            relative_path_error( const string &base, const string &rel, const string &error ) throw ();

        protected:
            wliteral const message() const throw ();
        };


    }


    template<>
    struct FOST_INET_DECLSPEC coercer< json, url > {
        json coerce( const url u ) {
            return json( u.as_string() );
        }
    };
    template<>
    struct FOST_INET_DECLSPEC coercer< url, json > {
        url coerce( const json u ) {
            return url( fostlib::coerce< string >( u ) );
        }
    };

    template<>
    struct FOST_INET_DECLSPEC coercer< url::filepath_string, string > {
        url::filepath_string coerce( const string &s );
    };


}


#endif // FOST_URL_HPP

