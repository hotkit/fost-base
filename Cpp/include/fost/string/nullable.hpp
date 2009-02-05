/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_NULLABLE_HPP
#define FOST_STRING_NULLABLE_HPP
#pragma once


#include <fost/nullable.hpp>
#include <fost/string.hpp>


namespace fostlib {


    template<>
    class nullable< utf8string > : private std::pair< bool, utf8string > {
    public:
        typedef utf8string t_value;

        inline nullable();
        inline nullable( const nullable & );
        inline nullable( const char *t );
        inline nullable( const utf8string & );
        inline nullable( t_null );

        template< typename Y >
        nullable( const nullable< Y > &y )
        : std::pair< bool, string >( y.isnull(), y.isnull() ? utf8string() : utf8string( y.value() ) ) {
        }

        inline ~nullable();

        inline bool isnull() const;

        inline const nullable &operator =( const char * );
        inline const nullable &operator =( const utf8string & );
        inline const nullable &operator =( t_null );
        inline bool operator ==( const char *rhs ) const;
        inline bool operator ==( const utf8string &rhs ) const;
        inline bool operator ==( const nullable< utf8string > &rhs ) const;
        inline bool operator !=( const char *rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const utf8string &rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const nullable< utf8string > &rhs ) const { return !( *this == rhs ); }

        inline void set_null();

        inline const utf8string &value() const;
        inline utf8string &value();
        inline const utf8string &value( const utf8string &value ) const;
    };
    template<>
    class nullable< string > : private std::pair< bool, string > {
    public:
        typedef string t_value;

        inline nullable();
        inline nullable( const nullable & );
        inline nullable( const wchar_t *t );
        inline nullable( const string & );
        inline nullable( t_null );

        template< typename Y >
        nullable( const nullable< Y > &y )
        : std::pair< bool, string >( y.isnull(), y.isnull() ? string() : string( y.value() ) ) {
        }

        inline ~nullable();

        inline bool isnull() const;

        inline const nullable &operator =( const wchar_t * );
        inline const nullable &operator =( const string & );
        inline const nullable &operator =( t_null );
        inline bool operator ==( const wchar_t *rhs ) const;
        inline bool operator ==( const string &rhs ) const;
        inline bool operator ==( const nullable< string > &rhs ) const;
        inline bool operator !=( const wchar_t *rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const string &rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const nullable< string > &rhs ) const { return !( *this == rhs ); }

        inline void set_null();

        inline const string &value() const;
        inline string &value();
        inline const string &value( const string &value ) const;
    };


}


/*
	fostlib::nullable Implementation
*/


#include <fost/exception/null.hpp>


inline fostlib::nullable< fostlib::utf8string >::nullable()
: std::pair< bool, fostlib::utf8string >( true, fostlib::utf8string() ) {
}
inline fostlib::nullable< fostlib::string >::nullable()
: std::pair< bool, fostlib::string >( true, fostlib::string() ) {
}

inline fostlib::nullable< fostlib::utf8string >::nullable( const fostlib::nullable< fostlib::utf8string > &t )
: std::pair< bool, fostlib::utf8string >( t.first, t.second ) {
}
inline fostlib::nullable< fostlib::string >::nullable( const fostlib::nullable< fostlib::string > &t )
: std::pair< bool, fostlib::string >( t.first, t.second ) {
}

inline fostlib::nullable< fostlib::utf8string >::nullable( const char *t )
: std::pair< bool, fostlib::utf8string >( false, t ) {
}
inline fostlib::nullable< fostlib::utf8string >::nullable( const fostlib::utf8string &t )
: std::pair< bool, fostlib::utf8string >( false, t ) {
}
inline fostlib::nullable< fostlib::string >::nullable( const wchar_t *t )
: std::pair< bool, fostlib::string >( false, t ) {
}
inline fostlib::nullable< fostlib::string >::nullable( const fostlib::string &t )
: std::pair< bool, fostlib::string>( false, t ) {
}

inline fostlib::nullable< fostlib::utf8string >::nullable( t_null )
: std::pair< bool, fostlib::utf8string >( true, fostlib::utf8string() ) {
}
inline fostlib::nullable< fostlib::string >::nullable( t_null )
: std::pair< bool, fostlib::string >( true, fostlib::string() ) {
}

inline fostlib::nullable< fostlib::utf8string >::~nullable() {
}
inline fostlib::nullable< fostlib::string >::~nullable() {
}

inline bool fostlib::nullable< fostlib::utf8string >::isnull() const {
	return first;
}
inline bool fostlib::nullable< fostlib::string >::isnull() const {
	return first;
}

inline void fostlib::nullable< fostlib::utf8string >::set_null() {
	first = true;
	second = fostlib::utf8string();
}
inline void fostlib::nullable< fostlib::string >::set_null() {
	first = true;
	second = fostlib::string();
}

inline const fostlib::nullable< fostlib::utf8string > &fostlib::nullable< fostlib::utf8string >::operator =( const char *t ) {
	first = false;
	second = t;
	return *this;
}
inline const fostlib::nullable< fostlib::utf8string > &fostlib::nullable< fostlib::utf8string >::operator =( const fostlib::utf8string &t ) {
	first = false;
	second = t;
	return *this;
}
inline const fostlib::nullable< fostlib::string > &fostlib::nullable< fostlib::string >::operator =( const wchar_t *t ) {
	first = false;
	second = t;
	return *this;
}
inline const fostlib::nullable< fostlib::string > &fostlib::nullable< fostlib::string >::operator =( const fostlib::string &t ) {
	first = false;
	second = t;
	return *this;
}

inline const fostlib::nullable< fostlib::utf8string > &fostlib::nullable< fostlib::utf8string >::operator =( t_null ) {
	set_null();
	return *this;
}
inline const fostlib::nullable< fostlib::string > &fostlib::nullable< fostlib::string >::operator =( t_null ) {
	set_null();
	return *this;
}

inline bool fostlib::nullable< fostlib::utf8string >::operator ==( const char *rhs ) const {
	if ( isnull() )
		return false;
	else
		return value() == rhs;
}
inline bool fostlib::nullable< fostlib::string >::operator ==( const wchar_t *rhs ) const {
	if ( isnull() )
		return false;
	else
		return value() == rhs;
}

inline bool fostlib::nullable< fostlib::utf8string >::operator ==( const fostlib::utf8string &rhs ) const {
	if ( isnull() ) {
		return false;
	} else {
		return value() == rhs;
	}
}
inline bool fostlib::nullable< fostlib::string >::operator ==( const fostlib::string &rhs ) const {
	if ( isnull() ) {
		return false;
	} else {
		return value() == rhs;
	}
}

inline bool fostlib::nullable< fostlib::utf8string >::operator ==( const fostlib::nullable< fostlib::utf8string > &rhs ) const {
	if ( isnull() && rhs.isnull() ) {
		return true;
	} else if ( !isnull() && !rhs.isnull() && value() == rhs.value() ) {
		return true;
	}
	return false;
}
inline bool fostlib::nullable< fostlib::string >::operator ==( const fostlib::nullable< fostlib::string > &rhs ) const {
	if ( isnull() && rhs.isnull() ) {
		return true;
	} else if ( !isnull() && !rhs.isnull() && value() == rhs.value() ) {
		return true;
	}
	return false;
}

inline const fostlib::utf8string &fostlib::nullable< fostlib::utf8string >::value() const {
	if ( isnull() )
        throw fostlib::exceptions::null();
	return second;
}
inline const fostlib::string &fostlib::nullable< fostlib::string >::value() const {
	if ( isnull() )
        throw fostlib::exceptions::null();
	return second;
}

inline fostlib::utf8string &fostlib::nullable< fostlib::utf8string >::value() {
	if ( isnull() )
        throw fostlib::exceptions::null();
	return second;
}
inline fostlib::string &fostlib::nullable< fostlib::string >::value() {
	if ( isnull() )
        throw fostlib::exceptions::null();
	return second;
}

inline const fostlib::utf8string &fostlib::nullable< fostlib::utf8string >::value( const fostlib::utf8string &v ) const {
	if ( isnull() ) return v;
	else return second;
}
inline const fostlib::string &fostlib::nullable< fostlib::string >::value( const fostlib::string &v ) const {
	if ( isnull() ) return v;
	else return second;
}


#endif // FOST_STRING_NULLABLE_HPP

