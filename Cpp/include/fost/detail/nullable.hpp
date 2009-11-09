/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_STRING_NULLABLE_HPP
#define FOST_STRING_NULLABLE_HPP
#pragma once


#include <fost/nullable.hpp>
#include <fost/detail/tagged-string.hpp>


namespace fostlib {


    template<>
    class nullable< utf8_string > : private std::pair< bool, utf8_string > {
    public:
        typedef utf8_string t_value;

        inline nullable();
        inline nullable( const nullable & );
        inline nullable( const char *t );
        inline nullable( const utf8_string & );
        inline nullable( t_null );

        template< typename Y >
        nullable( const nullable< Y > &y )
        : std::pair< bool, utf8_string >( y.isnull(), y.isnull() ? utf8_string() : utf8_string( y.value() ) ) {
        }

        inline ~nullable();

        inline bool isnull() const;

        inline const nullable &operator =( const char * );
        inline const nullable &operator =( const utf8_string & );
        inline const nullable &operator =( t_null );
        inline bool operator ==( const char *rhs ) const;
        inline bool operator ==( const utf8_string &rhs ) const;
        inline bool operator ==( const nullable< utf8_string > &rhs ) const;
        inline bool operator !=( const char *rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const utf8_string &rhs ) const { return !( *this == rhs ); }
        inline bool operator !=( const nullable< utf8_string > &rhs ) const { return !( *this == rhs ); }

        inline void set_null();

        inline const utf8_string &value() const;
        inline utf8_string &value();
        inline const utf8_string &value( const utf8_string &value ) const;
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


    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const tagged_string< S, I > &l,
        const nullable< tagged_string< S, I > > &r
    ) {
        if ( r.isnull() )
            return l;
        else
            return l + r.value();
    }
    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const tagged_string< S, I > &l,
        const tagged_string< S, I > &m,
        const nullable< tagged_string< S, I > > &r
    ) {
        if ( r.isnull() )
            return l;
        else
            return l + m + r.value();
    }
    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const nullable< tagged_string< S, I > > &l,
        const tagged_string< S, I > &m,
        const nullable< tagged_string< S, I > > &r
    ) {
        if ( l.isnull() )
            return r;
        else if ( r.isnull() )
            return l;
        return l.value() + m + r.value();
    }
    template< typename S, typename I >
    nullable< tagged_string< S, I > > concat(
        const nullable< tagged_string< S, I > > &l,
        const tagged_string< S, I > &m,
        const tagged_string< S, I > &r
    ) {
        if ( l.isnull() )
            return r;
        return l.value() + m + r;
    }


}


/*
    fostlib::nullable Implementation
*/


#include <fost/exception/null.hpp>


inline fostlib::nullable< fostlib::utf8_string >::nullable()
: std::pair< bool, fostlib::utf8_string >( true, fostlib::utf8_string() ) {
}
inline fostlib::nullable< fostlib::string >::nullable()
: std::pair< bool, fostlib::string >( true, fostlib::string() ) {
}

inline fostlib::nullable< fostlib::utf8_string >::nullable( const fostlib::nullable< fostlib::utf8_string > &t )
: std::pair< bool, fostlib::utf8_string >( t.first, t.second ) {
}
inline fostlib::nullable< fostlib::string >::nullable( const fostlib::nullable< fostlib::string > &t )
: std::pair< bool, fostlib::string >( t.first, t.second ) {
}

inline fostlib::nullable< fostlib::utf8_string >::nullable( const char *t )
: std::pair< bool, fostlib::utf8_string >( false, t ) {
}
inline fostlib::nullable< fostlib::utf8_string >::nullable( const fostlib::utf8_string &t )
: std::pair< bool, fostlib::utf8_string >( false, t ) {
}
inline fostlib::nullable< fostlib::string >::nullable( const wchar_t *t )
: std::pair< bool, fostlib::string >( false, t ) {
}
inline fostlib::nullable< fostlib::string >::nullable( const fostlib::string &t )
: std::pair< bool, fostlib::string>( false, t ) {
}

inline fostlib::nullable< fostlib::utf8_string >::nullable( t_null )
: std::pair< bool, fostlib::utf8_string >( true, fostlib::utf8_string() ) {
}
inline fostlib::nullable< fostlib::string >::nullable( t_null )
: std::pair< bool, fostlib::string >( true, fostlib::string() ) {
}

inline fostlib::nullable< fostlib::utf8_string >::~nullable() {
}
inline fostlib::nullable< fostlib::string >::~nullable() {
}

inline bool fostlib::nullable< fostlib::utf8_string >::isnull() const {
    return first;
}
inline bool fostlib::nullable< fostlib::string >::isnull() const {
    return first;
}

inline void fostlib::nullable< fostlib::utf8_string >::set_null() {
    first = true;
    second = fostlib::utf8_string();
}
inline void fostlib::nullable< fostlib::string >::set_null() {
    first = true;
    second = fostlib::string();
}

inline const fostlib::nullable< fostlib::utf8_string > &fostlib::nullable< fostlib::utf8_string >::operator =( const char *t ) {
    first = false;
    second = t;
    return *this;
}
inline const fostlib::nullable< fostlib::utf8_string > &fostlib::nullable< fostlib::utf8_string >::operator =( const fostlib::utf8_string &t ) {
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

inline const fostlib::nullable< fostlib::utf8_string > &fostlib::nullable< fostlib::utf8_string >::operator =( t_null ) {
    set_null();
    return *this;
}
inline const fostlib::nullable< fostlib::string > &fostlib::nullable< fostlib::string >::operator =( t_null ) {
    set_null();
    return *this;
}

inline bool fostlib::nullable< fostlib::utf8_string >::operator ==( const char *rhs ) const {
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

inline bool fostlib::nullable< fostlib::utf8_string >::operator ==( const fostlib::utf8_string &rhs ) const {
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

inline bool fostlib::nullable< fostlib::utf8_string >::operator ==( const fostlib::nullable< fostlib::utf8_string > &rhs ) const {
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

inline const fostlib::utf8_string &fostlib::nullable< fostlib::utf8_string >::value() const {
    if ( isnull() )
        throw fostlib::exceptions::null();
    return second;
}
inline const fostlib::string &fostlib::nullable< fostlib::string >::value() const {
    if ( isnull() )
        throw fostlib::exceptions::null();
    return second;
}

inline fostlib::utf8_string &fostlib::nullable< fostlib::utf8_string >::value() {
    if ( isnull() )
        throw fostlib::exceptions::null();
    return second;
}
inline fostlib::string &fostlib::nullable< fostlib::string >::value() {
    if ( isnull() )
        throw fostlib::exceptions::null();
    return second;
}

inline const fostlib::utf8_string &fostlib::nullable< fostlib::utf8_string >::value( const fostlib::utf8_string &v ) const {
    if ( isnull() ) return v;
    else return second;
}
inline const fostlib::string &fostlib::nullable< fostlib::string >::value( const fostlib::string &v ) const {
    if ( isnull() ) return v;
    else return second;
}


#endif // FOST_STRING_NULLABLE_HPP

