/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DB_HPP
#define FOST_DB_HPP
#pragma once


#include <fost/core>


namespace fostlib {


    namespace exceptions {


        class FOST_SCHEMA_DECLSPEC data_driver : public exception {
        public:
            data_driver( const string &error, const string &driver ) throw ();
            data_driver( const string &error, const string &driver1, const string &driver2 ) throw ();

        protected:
            wliteral const message() const throw ();
        };


        class FOST_SCHEMA_DECLSPEC transaction_fault : public exception {
        public:
            transaction_fault( const string &error ) throw ();

        protected:
            wliteral const message() const throw ();
        };


    }

    class dbconnection;
    class recordset;


    class FOST_SCHEMA_DECLSPEC dbinterface {
    protected:
        explicit dbinterface( const string &driver_name );
    public:
        class read;
        class recordset;
        class write;

        static const dbinterface &connection( const string &read, const nullable< string > &write );

        virtual void create_database( dbconnection &dbc, const string &name ) const = 0;
        virtual void drop_database( dbconnection &dbc, const string &name ) const = 0;

        virtual boost::shared_ptr< fostlib::dbinterface::read > reader( dbconnection &dbc ) const = 0;

        class FOST_SCHEMA_DECLSPEC read : boost::noncopyable {
        protected:
            read( dbconnection &dbc );
        public:
            virtual ~read();

            virtual boost::shared_ptr< write > writer() = 0;
            virtual boost::shared_ptr< recordset > query( const fostlib::string & ) const = 0;

        protected:
            dbconnection &m_connection;
        private:
            bool m_inTransaction;
            friend class fostlib::dbinterface::write;
        };
        class FOST_SCHEMA_DECLSPEC write : boost::noncopyable {
        protected:
            write( read &reader );
        public:
            virtual ~write();

            virtual void create_table( const fostlib::string &table, const std::list< std::pair< fostlib::string, fostlib::string > > &key, const std::list< std::pair< fostlib::string, fostlib::string > > &columns ) = 0;
            virtual void drop_table( const fostlib::string &table ) = 0;

            virtual void execute( const fostlib::string & ) = 0;
            virtual void commit() = 0;
            virtual void rollback() = 0;

        protected:
            dbconnection &m_connection;
            read &m_reader;
        };

        class FOST_SCHEMA_DECLSPEC recordset : boost::noncopyable {
        protected:
            recordset( const dbinterface &dbi, const fostlib::string &cmd );
        public:
            fostlib::accessors< const fostlib::string > command;

            virtual ~recordset();

            virtual bool eof() const = 0;
            virtual void moveNext() = 0;
            virtual std::size_t fields() const = 0;
            virtual const fostlib::string &name( std::size_t f ) const = 0;
            virtual const json &field( std::size_t i ) const = 0;
            virtual const json &field( const string &name ) const = 0;
        };
    };


    class FOST_SCHEMA_DECLSPEC recordset {
    public:
        recordset( boost::shared_ptr< dbinterface::recordset > rs );
        ~recordset();

        const fostlib::string &command() const {
            return m_interface->command();
        }
        bool eof() const;
        void moveNext();

        std::size_t fields() const;
        const fostlib::string &name( std::size_t f ) const;
        const json &field( const fostlib::string &i ) const;
        const json &field( std::size_t i ) const;

        bool isnull( const fostlib::string & ) const;
        bool isnull( std::size_t ) const;

    private:
        boost::shared_ptr< dbinterface::recordset > m_interface;
    };


    class FOST_SCHEMA_DECLSPEC dbtransaction : boost::noncopyable {
    public:
        dbtransaction( dbconnection &dbc );
        ~dbtransaction();

        void create_table( const string &table, const std::list< std::pair< string, string > > &key, const std::list< std::pair< string, string > > &columns );
        void drop_table( const string &table );

        dbtransaction &execute( const string & );
        void commit();

    private:
        dbconnection &m_connection;
        boost::shared_ptr< dbinterface::write > m_transaction;
    };


    class FOST_SCHEMA_DECLSPEC dbconnection : boost::noncopyable {
        friend class dbtransaction;
    public:
        static const setting< bool > c_commitCount;
        static const setting< int > c_commitCountDomain;

        dbconnection( const string &readDSN );
        dbconnection( const string &readDSN, const fostlib::string &writeDSN );
        ~dbconnection();

        const dbinterface &driver() const;

        void create_database( const string &name );
        void drop_database( const string &name );

        recordset query( const string &command );

        bool in_transaction() const;
        dbtransaction &transaction();

        accessors< const string > readDSN;
        accessors< const nullable< string > > writeDSN;

    private:
        bool m_readOnly;
        const dbinterface &m_interface;
        boost::shared_ptr< dbinterface::read > m_connection;
        dbtransaction *m_transaction;
    };


}


#endif // FOST_DB_HPP
