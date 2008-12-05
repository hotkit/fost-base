/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
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
        meta_attribute( const string &name, const field_base &type, bool not_null,
            const nullable< std::size_t > &size, const nullable< std::size_t > &precision
        );

        accessors< const string > name;
        const field_base &type() const;
        accessors< const bool > not_null;
        accessors< const nullable< std::size_t > > size;
        accessors< const nullable< std::size_t > > precision;

        virtual boost::shared_ptr< attribute_base > construct() const = 0;

    private:
        const field_base &m_type;
    };


    class instance;

    class FOST_SCHEMA_DECLSPEC meta_instance : boost::noncopyable {
    public:
        explicit meta_instance( const string &name );
        virtual ~meta_instance() {}

        accessors< const string > name;
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

        boost::shared_ptr< instance > operator() () const;

    private:
        typedef std::vector< boost::shared_ptr< meta_attribute > > columns_type;
        columns_type m_keys, m_columns;
    };


    class FOST_SCHEMA_DECLSPEC attribute_base : boost::noncopyable {
    protected:
        attribute_base( meta_attribute & );
    public:
        virtual ~attribute_base() {}

        const meta_attribute &_meta() const;
    private:
        const meta_attribute &m_meta;
    };


    class FOST_SCHEMA_DECLSPEC instance : boost::noncopyable {
        friend class meta_instance;
    protected:
        instance( const meta_instance & );
    public:
        void attribute( boost::shared_ptr< attribute_base > attr );
        virtual ~instance() {}

        const meta_instance &_meta() const;

        attribute_base &operator []( const string &name );
        const attribute_base &operator []( const string &name ) const {
            instance *self = const_cast< instance * >( this );
            return (*self)[ name ];
        }

    private:
        const meta_instance &m_meta;
        typedef std::map< string, boost::shared_ptr< attribute_base > > attributes_type;
        attributes_type m_attributes;
    };


}


#endif // FOST_SCHEMA_DYNAMIC_HPP
