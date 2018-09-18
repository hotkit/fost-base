/**
    Copyright 2001-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_SETTINGS_HPP
#define FOST_SETTINGS_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/json.hpp>
#include <fost/accessors.hpp>
#include <boost/filesystem.hpp>


namespace fostlib {


    template< typename V >
    class setting;

    template<>
    class FOST_CORE_DECLSPEC setting< json > : public boost::noncopyable {
    public:
        typedef std::list< std::pair< string, json > > t_definitions;
        typedef std::pair< string, t_definitions > t_value;
        typedef std::list< t_value > t_values;
        typedef std::pair< string, t_values > t_section;
        typedef std::list< t_section > t_sections;

        template< typename T >
        setting( const string &domain, const setting &setting, const T &value )
        : domain( setting.domain() ), section( setting.section() ), name( setting.name() ), m_value( json( value ) ) {
            construct( domain, section(), name(), m_value, false );
        }
        template< typename T >
        setting( const string &domain, const string &section, const string &name, const T &value, bool def = false )
        : domain( domain ), section( section ), name( name ), m_value( json( value ) ) {
            construct( domain, section, name, m_value, def );
        }
        virtual ~setting();

        ostream &print_on( ostream & ) const;

        fostlib::accessors< const string > domain, section, name;
        json value() const;

        static json value( const string &section, const string &name );
        static bool exists( const string &section, const string &name );
        static nullable< json > value( const string &section, const string &name, const nullable< json > &a_default );

        static ostream &printAllOn( ostream & );
        static t_sections current();

    private:
        void construct( const string &domain, const string &section, const string &name, const json &value, bool def );
        const json m_value;
    };

    template< typename V >
    class setting : public setting< json > {
        typedef setting< json > superclass_t;
    public:
        typedef typename superclass_t::t_definitions t_definitions;
        typedef typename superclass_t::t_value t_value;
        typedef typename superclass_t::t_values t_values;
        typedef typename superclass_t::t_section t_section;
        typedef typename superclass_t::t_sections t_sections;
        typedef V t_final_value;

        setting( const string &domain, const setting< t_final_value > &setting, const t_final_value &value )
        : superclass_t( domain, setting, value ) {
        }
        setting( const string &domain, const string &section, const string &name, const t_final_value &value, bool def = false )
        : superclass_t( domain, section, name, value, def ) {
        }

        t_final_value value() const {
            return coerce< t_final_value >( superclass_t::value() );
        }

        static t_final_value value( nliteral section, nliteral name ) {
            return value(string(section), string(name));
        }
        static t_final_value value( wliteral section, wliteral name ) {
            return value(string(section), string(name));
        }
        static t_final_value value( const string &section, const string &name ) {
            return coerce< t_final_value >( superclass_t::value( section, name ) );
        }
        static nullable< t_final_value > value( const string &section, const string &name, const nullable< t_final_value > &def ) {
            nullable< fostlib::json > json;
            if ( not def ) {
                json = superclass_t::value( section, name, null );
            } else {
                json = superclass_t::value( section, name, fostlib::json( def.value() ) );
            }
            if ( not json ) {
                return null;
            } else {
                return coerce< t_final_value >(*json);
            }
        }
    };


    /// Store a number of settings read from the passed in JSON blob
    class FOST_CORE_DECLSPEC settings {
        std::list< boost::shared_ptr< setting< json > > > m_settings;
        void load_settings(const string &domain,
            const boost::filesystem::path &);
        void load_settings(const string &domain, const json &);
    public:
        /// Construct the setting store for the given domain
        settings(const string &domain, const json &);
        /// Construct the settings given a JSON file in the specified setting
        settings(const setting<string> &);
        /// Construct the settings given a filename containing JSON
        settings(const boost::filesystem::path &);
    };


}


#endif // FOST_SETTINGS_HPP

