/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_SCHEMA_ATTRIBUTES_HPP
#define FOST_SCHEMA_ATTRIBUTES_HPP
#pragma once


#include <fost/schema/fields.hpp>
#include <fost/schema/dynamic.hpp>

#include <fost/exception/not_implemented.hpp>


namespace fostlib {


    template< typename value_type >
    class field_wrapper : public field_base {
    public:
        class value : public attribute_base {
        public:
            value( const meta_attribute &m ) : attribute_base( m ), m_value( value_type() ) {}

            json to_json() const {
                return coerce< json >( m_value );
            }
            void from_json( const json &j ) {
                m_value = coerce< value_type >( j );
            }

        private:
            value_type m_value;
        };
        class nullable : public attribute_base {
        public:
            nullable( const meta_attribute &m ) : attribute_base( m ) {}

            json to_json() const {
                if ( m_value.isnull() )
                    return json();
                else
                    return coerce< json >( m_value.value() );
            }
            void from_json( const json &j ) {
                if ( j.isnull() )
                    m_value = null;
                else
                    m_value = coerce< value_type >( j );
            }

        private:
            fostlib::nullable< value_type > m_value;
        };
    private:
        struct factory : public meta_attribute {
            factory(
                const string &name, const field_base &type, bool not_null,
                const fostlib::nullable< std::size_t > &size, const fostlib::nullable< std::size_t > &precision
            ) : meta_attribute( name, type, not_null, size, precision ) {
            }

            boost::shared_ptr< attribute_base > construct() const {
                if ( not_null() )
                    return boost::shared_ptr< attribute_base >( new value( *this ) );
                else
                    return boost::shared_ptr< attribute_base >( new nullable( *this ) );
            }
        };
    public:
        field_wrapper( const string &type_name )
        : field_base( type_name ) {
        }

        boost::shared_ptr< meta_attribute > meta_maker(
            const string &name, bool not_null,
            const fostlib::nullable< std::size_t > &size, const fostlib::nullable< std::size_t > &precision
        ) const {
            return boost::shared_ptr< meta_attribute >( new factory( name, *this, not_null, size, precision ) );
        }
    };


    extern const field_wrapper< int64_t > integer;
    extern const field_wrapper< string > varchar, text;


}


#endif // FOST_SCHEMA_ATTRIBUTES_HPP
