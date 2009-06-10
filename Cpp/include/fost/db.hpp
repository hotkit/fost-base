/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
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


    }

    // Forward declarations of classes in schema/dynamic.hpp
    class instance;
    class meta_instance;
    // Forward declarations of classes in this file
    class dbconnection;
    class recordset;


    /*
        Specifies the database interface that all database drivers must implement
    */
    class FOST_SCHEMA_DECLSPEC dbinterface {
    protected:
        explicit dbinterface( const string &driver_name );
    public:
        class read;
        class recordset;
        class write;

        static const dbinterface &connection( const json &parameters );
        static const dbinterface &connection( const string &read, const nullable< string > &write );

        virtual void create_database( dbconnection &dbc, const string &name ) const = 0;
        virtual void drop_database( dbconnection &dbc, const string &name ) const = 0;

        virtual boost::shared_ptr< fostlib::dbinterface::read > reader( dbconnection &dbc ) const = 0;

        /*
            These members implement ID creation functionality
        */
        virtual int64_t next_id( dbconnection &dbc, const string &counter ) const = 0;
        virtual int64_t current_id( dbconnection &dbc, const string &counter ) const = 0;
        virtual void used_id( dbconnection &dbc, const string &counter, int64_t value ) const = 0;

        /*
            Every database driver must implement these interfaces
        */
        class FOST_SCHEMA_DECLSPEC read : boost::noncopyable {
        protected:
            read( dbconnection &dbc );
        public:
            virtual ~read();

            virtual boost::shared_ptr< write > writer() = 0;

            virtual boost::shared_ptr< recordset > query( const meta_instance &item, const json &key ) const = 0;
            virtual boost::shared_ptr< recordset > query( const string &cmd ) const = 0;

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

            virtual void create_table( const meta_instance &definition ) = 0;
            virtual void drop_table( const meta_instance &definition ) = 0;
            virtual void drop_table( const fostlib::string &table ) = 0;

            virtual void insert( const instance &object ) = 0;
            virtual void execute( const string &cmd ) = 0;

            virtual void commit() = 0;
            virtual void rollback() = 0;

        protected:
            dbconnection &m_connection;
            read &m_reader;
        };

        class FOST_SCHEMA_DECLSPEC recordset : boost::noncopyable {
        protected:
            recordset( const fostlib::string &cmd );
        public:
            fostlib::accessors< const fostlib::string > command;

            virtual ~recordset();

            virtual bool eof() const = 0;
            virtual void moveNext() = 0;
            virtual std::size_t fields() const = 0;
            virtual const fostlib::string &name( std::size_t f ) const = 0;
            virtual const json &field( std::size_t i ) const = 0;
            virtual const json &field( const string &name ) const = 0;

            virtual json to_json() const = 0;
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

        json to_json() const;

    private:
        boost::shared_ptr< dbinterface::recordset > m_interface;
    };


    class FOST_SCHEMA_DECLSPEC dbtransaction : boost::noncopyable {
    public:
        dbtransaction( dbconnection &dbc );
        ~dbtransaction();

        void create_table( const meta_instance &meta );
        void drop_table( const meta_instance &meta );
        void drop_table( const string &table );

        dbtransaction &insert( const instance &object, boost::function< void( void ) > oncommit );
        void commit();

    private:
        dbconnection &m_connection;
        boost::shared_ptr< dbinterface::write > m_transaction;
        std::vector< boost::function< void( void ) > > m_oncommit, m_onfail;
    };


    class FOST_SCHEMA_DECLSPEC dbconnection : boost::noncopyable {
        friend class dbtransaction;
    public:
        static const setting< bool > c_commitCount;
        static const setting< string > c_commitCountDomain;

        explicit dbconnection( const json &configuration );
        explicit dbconnection( const string &read_dsn );
        dbconnection( const string &read_dsn, const fostlib::string &write_dsn );
        ~dbconnection();

        const dbinterface &driver() const;

        void create_database( const string &name );
        void drop_database( const string &name );

        int64_t next_id( const string &counter );
        int64_t current_id( const string &counter );
        void used_id( const string &counter, int64_t value );

        recordset query( const meta_instance &item, const json &key = json() );
        recordset query( const string &cmd );

        bool in_transaction() const;
        dbtransaction &transaction();

        accessors< const json > configuration;

    private:
        const dbinterface &m_interface;
        boost::shared_ptr< dbinterface::read > m_connection;
        dbtransaction *m_transaction;
    };


}


#endif // FOST_DB_HPP
