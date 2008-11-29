/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_INTERNET_MIME_HPP
#define FOST_INTERNET_MIME_HPP
#pragma once


#include <fost/core>


namespace fostlib {


    class FOST_INET_DECLSPEC FSL_ABSTRACT headers_base {
    public:
        headers_base();

        class content;

        void parse( const string &headers );

        bool exists( const string & ) const;
        content &add( const string &name, const content & );
        const content &operator[]( const string & ) const;

        typedef std::map< string, content >::const_iterator const_iterator;
        const_iterator begin() const;
        const_iterator end() const;

        class FOST_INET_DECLSPEC content {
        public:
            content();
            content( wliteral );
            content( const string & );
            content( const string &, const std::map< string, string > & );

            accessors< string > value;

            content &subvalue( const string &k, const string &v );
            nullable< string > subvalue( const string &k ) const;

            typedef std::map< string, string >::const_iterator const_iterator;
            const_iterator begin() const;
            const_iterator end() const;

        private:
            std::map< string, string > m_subvalues;
        };

    protected:
        virtual std::pair< string, content > value( const string &name, const string &value ) = 0;

    private:
        std::map< string, content > m_headers;
    };


    /*
        This initial implementation is only intended to be able to parse HTTP POST multipart/form-data encoded data
    */
    class FOST_INET_DECLSPEC mime : boost::noncopyable {
    public:
        class FOST_INET_DECLSPEC mime_headers : public fostlib::headers_base {
        protected:
            std::pair< string, headers_base::content > value( const string &name, const string &value );
        };

        mime( const string &content_type = L"multipart/mixed" );
        explicit mime( const mime_headers &headers, const string &content_type = L"multipart/mixed" );
        virtual ~mime();

        accessors< string > content_type;
        accessors< mime_headers, fostlib::lvalue > headers;
        accessors< std::list< boost::shared_ptr< mime > >, fostlib::lvalue > items;

        virtual std::ostream &print_on( std::ostream & ) const;

        virtual bool boundary_is_ok( const string &boundary ) const;
    };


    FOST_INET_DECLSPEC std::ostream &operator <<( std::ostream &o, const headers_base &headers );
    FOST_INET_DECLSPEC std::ostream &operator <<( std::ostream &o, const headers_base::content &value );
    inline std::ostream &operator <<( std::ostream &o, const mime &m ) {
        return m.print_on( o );
    }


}


#endif // FOST_INTERNET_MIME_HPP
