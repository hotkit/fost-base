/*
    Copyright 2007-2014, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#ifdef WIN32
    #pragma warning ( push )
    #pragma warning ( disable : 4244 ) // Boost 1.35 pre release - warning C4244: '=' : conversion from '__w64 int' to 'unsigned int', possible loss of data
    #pragma warning ( disable : 4267 ) // Boost 1.35 pre release - warning C4267: 'return' : conversion from 'size_t' to 'int', possible loss of data
    #pragma warning ( disable : 4311 ) // Boost 1.35 pre release - warning C4311: 'type cast' : pointer truncation from 'void *const ' to 'long'
    #pragma warning ( disable : 4312 ) // Boost 1.35 pre release - warning C4312: 'type cast' : conversion from 'long' to 'void *' of greater size
#endif
#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>
#ifdef WIN32
    #pragma warning ( pop )
#endif
#include <fost/exception/missing_setting.hpp>
#include <fost/file.hpp>


using namespace fostlib;


#define WRITELOCKONLY


namespace {


    typedef std::list< setting< json > * > t_settingStack;
    typedef std::pair< string, string > t_settingKey;
    typedef std::map< t_settingKey, t_settingStack > t_settings;


#ifdef WRITELOCKONLY
    boost::mutex &g_mutex() {
        static boost::mutex mutex;
        return mutex;
    }
#else
    ExclusiveWrite &g_exw() {
        static ExclusiveWrite exw( 20 );
        return exw;
    }
#endif


    t_settings &g_settingsSettings() {
        static t_settings settings;
        return settings;
    }


    inline
#ifdef WRITELOCKONLY
    t_settings &g_writeSettings( boost::mutex::scoped_lock & ) {
#else
    t_settings &g_writeSettings( ExclusiveWrite::WriteLock & ) {
#endif
        return g_settingsSettings();
    }
    inline
#ifdef WRITELOCKONLY
    const t_settings &g_readSettings( boost::mutex::scoped_lock & ) {
#else
    const t_settings &g_readSettings( ExclusiveWrite::ReadLock & ) {
#endif
        return g_settingsSettings();
    }


}


void fostlib::setting< json >::construct( const string &/*domain*/, const string &section, const string &name, const json &/*value*/, bool def ) {
#ifdef WRITELOCKONLY
    boost::mutex::scoped_lock lock( g_mutex() );
#else
    ExclusiveWrite::WriteLock lock( g_exw() );
#endif
    if ( def )
        g_writeSettings( lock )[ std::make_pair( section, name ) ].push_front( this );
    else
        g_writeSettings( lock )[ std::make_pair( section, name ) ].push_back( this );
}


fostlib::setting< json >::~setting() {
    try {
#ifdef WRITELOCKONLY
        boost::mutex::scoped_lock lock( g_mutex() );
#else
        ExclusiveWrite::WriteLock lock( g_exw() );
#endif
        t_settings::iterator pos( g_writeSettings( lock ).find( std::make_pair( section(), name() ) ) );
        if ( pos == g_writeSettings( lock ).end() ) {
            //throw Exceptions::MissingSetting( m_section, m_name );
            // This is bad, but we don't want to be throwing an exception here, but we can log losing one
            // Lose exception
            absorb_exception();
        } else {
            (*pos).second.remove( this );
            // Now check to see if the section/name is empty. If so then we ought to remove that too.
            if ( (*pos).second.empty() ) {
                g_writeSettings( lock ).erase( pos );
            }
        }
    } catch ( fostlib::exceptions::exception & ) {
        absorb_exception();
    }
}


ostream &fostlib::setting< json >::print_on( ostream &o ) const {
    return o << L"setting," << section() << L"," << name() << L","
        << json::unparse(m_value, false) << L"," << domain();
}


json fostlib::setting< json >::value() const {
    return value( section(), name() );
}


json fostlib::setting< json >::value( const string &section, const string &name ) {
#ifdef WRITELOCKONLY
    boost::mutex::scoped_lock lock( g_mutex() );
#else
    ExclusiveWrite::ReadLock lock( g_exw() );
#endif
    t_settings::const_iterator pos( g_readSettings( lock ).find( std::make_pair( section, name ) ) );
    if ( pos == g_readSettings( lock ).end() ) {
        throw fostlib::exceptions::missing_setting( section, name );
    } else {
        if ( (*pos).second.rbegin() == (*pos).second.rend() ) {
            throw fostlib::exceptions::missing_setting( section, name );
        } else {
            return (*(*pos).second.rbegin())->m_value;
        }
    }
}


bool fostlib::setting< json >::exists( const string &section, const string &name ) {
    return !setting::value( section, name, null ).isnull();
}


nullable< json > fostlib::setting< json >::value( const string &section, const string &name, const nullable< json > &def ) {
#ifdef WRITELOCKONLY
    boost::mutex::scoped_lock lock( g_mutex() );
#else
    ExclusiveWrite::ReadLock lock( g_exw() );
#endif
    t_settings::const_iterator pos( g_readSettings( lock ).find( std::make_pair( section, name ) ) );
    if ( pos == g_readSettings( lock ).end() ) {
        return def;
    } else {
        if ( (*pos).second.rbegin() == (*pos).second.rend() ) {
            return def;
        } else {
            return (*(*pos).second.rbegin())->m_value;
        }
    }
}


fostlib::setting< json >::t_sections fostlib::setting< json >::current() {
    t_sections sections;
    string section;
#ifdef WRITELOCKONLY
    boost::mutex::scoped_lock lock( g_mutex() );
#else
    ExclusiveWrite::ReadLock lock( g_exw() );
#endif
    for ( t_settings::const_iterator setting( g_readSettings( lock ).begin() ); setting != g_readSettings( lock ).end(); ++setting ) {
        if ( (*setting).first.first != section ) {
            section = (*setting).first.first;
            sections.push_back( make_pair( section, t_values() ) );
        }
        sections.back().second.push_back( make_pair( (*setting).first.second, t_definitions() ) );
        const t_settingStack stack( (*setting).second );
        for ( t_settingStack::const_iterator val( stack.begin() ); val != stack.end(); ++val ) {
            sections.back().second.back().second.push_back( std::make_pair( (*val)->domain(), (*val)->m_value ) );
        }
    }
    return sections;
}


ostream &fostlib::setting< json >::printAllOn( ostream &o ) {
#ifdef WRITELOCKONLY
    boost::mutex::scoped_lock lock( g_mutex() );
#else
    ExclusiveWrite::ReadLock lock( g_exw() );
#endif
    for ( t_settings::const_iterator setting( g_readSettings( lock ).begin() ); setting != g_readSettings( lock ).end(); ++setting ) {
        const t_settingStack stack( (*setting).second );
        for ( t_settingStack::const_iterator val( stack.begin() ); val != stack.end(); ) {
            (*val)->print_on( o );
            o << ( ++val == stack.end() ? L",Used" : L",Overridden" ) << std::endl;
        }
    }
    return o;
}


fostlib::settings::settings(const string &domain, const json &values) {
    load_settings(domain, values);
}


fostlib::settings::settings(const boost::filesystem::wpath &file ) {
    load_settings(coerce<string>(file), file);
}


fostlib::settings::settings(const setting<string> &json_file) {
    load_settings(json_file.value(),
        coerce<boost::filesystem::wpath>(json_file.value()));
}


void fostlib::settings::load_settings(
    const string &domain, const boost::filesystem::wpath &filename
) {
    load_settings(domain, fostlib::json::parse(fostlib::utf::load_file(
        filename, "{}")));
}

void fostlib::settings::load_settings(
    const string &domain, const json &values
) {
    if ( values.isobject() ) {
        for ( json::const_iterator section(values.begin());
                section != values.end(); ++section ) {
            if ( section->isobject() ) {
                for ( json::const_iterator name(section->begin());
                        name != section->end(); ++name ) {
                    m_settings.push_back(
                        boost::shared_ptr< setting<json> >(
                            new setting<json>(
                                domain, coerce<string>(section.key()),
                                coerce<string>(name.key()), *name)));
                }
            }
        }
    }
}

