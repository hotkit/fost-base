/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_DYNAMIC_HPP
#define FOST_SCHEMA_DYNAMIC_HPP
#pragma once


#include <fost/schema/fields.hpp>


namespace fostlib {


    class attribute_base;

    class FOST_SCHEMA_DECLSPEC meta_attribute : boost::noncopyable {
    public:
        meta_attribute( const string &name, const field_base &type, bool key, bool not_null,
            const nullable< std::size_t > &size, const nullable< std::size_t > &precision
        );

        accessors< const string > name;
        const field_base &type() const;
        accessors< const bool > key;
        accessors< const bool > not_null;
        accessors< const nullable< std::size_t > > size;
        accessors< const nullable< std::size_t > > precision;

        virtual boost::shared_ptr< attribute_base > construct() const = 0;
        virtual boost::shared_ptr< attribute_base > construct( const json & ) const = 0;

    private:
        const field_base &m_type;
    };


    class FOST_SCHEMA_DECLSPEC enclosure : boost::noncopyable {
    public:
        explicit enclosure( const string &name );
        enclosure( const enclosure &enc, const string &name );
        virtual ~enclosure() {}

        accessors< const string > name;
        string fq_name( const string &delim = L"." ) const;
        const enclosure &parent() const;

        bool in_global() const { return m_parent == NULL; }

        static const enclosure global;

    private:
        const enclosure * const m_parent;
    };


    class instance;
    class dbconnection;
    class FOST_SCHEMA_DECLSPEC meta_instance : public enclosure {
        typedef std::vector< boost::shared_ptr< meta_attribute > > columns_type;
    public:
        explicit meta_instance( const string &name );
        meta_instance( const enclosure &enc, const string &name );

        const meta_attribute &operator[]( const string &name ) const;

        meta_instance &primary_key(
            const string &name, const string &type,
            const nullable< std::size_t > & size = null, const nullable< std::size_t > &precision = null
        );
        meta_instance &field(
            const string &name, const string &type, bool not_null,
            const nullable< std::size_t > & size = null, const nullable< std::size_t > &precision = null
        );

        virtual string table( const instance & ) const;

        boost::shared_ptr< instance > create( dbconnection &dbc ) const;
        boost::shared_ptr< instance > create( dbconnection &dbc, const json & ) const;

        typedef columns_type::const_iterator const_iterator;
        const_iterator begin() const { return m_columns.begin(); }
        const_iterator end() const { return m_columns.end(); }

    private:
        columns_type m_columns;
    };


    class FOST_SCHEMA_DECLSPEC attribute_base : boost::noncopyable {
    protected:
        attribute_base( const meta_attribute & );
    public:
        virtual ~attribute_base() {}

        virtual json to_json() const = 0;
        virtual void from_json( const json & ) = 0;

        const meta_attribute &_meta() const;
    private:
        const meta_attribute &m_meta;
    };


    class FOST_SCHEMA_DECLSPEC instance : boost::noncopyable {
        friend class meta_instance;
    protected:
        instance( dbconnection &, const meta_instance & );
    public:
        typedef std::vector< boost::shared_ptr< attribute_base > > key_type;

        void attribute( boost::shared_ptr< attribute_base > attr );
        virtual ~instance() {}

        const meta_instance &_meta() const;

        attribute_base &operator []( const string &name );
        const attribute_base &operator []( const string &name ) const {
            instance *self = const_cast< instance * >( this );
            return (*self)[ name ];
        }

        bool in_database() const { return m_in_database; }
        void save();

    private:
        bool m_in_database, m_to_die;
        const meta_instance &m_meta;
        dbconnection *m_dbc;
        typedef std::map< string, boost::shared_ptr< attribute_base > > attributes_type;
        attributes_type m_attributes;
    };


}


#endif // FOST_SCHEMA_DYNAMIC_HPP
