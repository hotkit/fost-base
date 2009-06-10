/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_DB_DRIVER_HPP
#define FOST_DB_DRIVER_HPP
#pragma once


#include <fost/core>
#include <fost/db.hpp>


namespace fostlib {


    /*
        Every database driver must implement these interfaces
    */
    class FOST_SCHEMA_DECLSPEC dbinterface::read : boost::noncopyable {
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


    class FOST_SCHEMA_DECLSPEC dbinterface::write : boost::noncopyable {
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


    class FOST_SCHEMA_DECLSPEC dbinterface::recordset : boost::noncopyable {
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


}


#endif // FOST_DB_DRIVER_HPP
