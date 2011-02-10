/*
    Copyright 2007-2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_JSON_HPP
#define FOST_JSON_HPP
#pragma once


#include <fost/json-core.hpp>
#include <fost/variant.hpp>
#include <fost/coerce/ints.hpp>


namespace fostlib {


    template<> inline
    nullable< json::atom_t > json::get() const {
        const atom_t *a = boost::get< atom_t >( &m_element );
        if ( a )
            return *a;
        else
            return null;
    }
    template<> inline
    nullable< json::object_t > json::get() const {
        const object_t *o = boost::get< object_t >( &m_element );
        if ( o )
            return *o;
        else
            return null;
    }


    class FOST_CORE_DECLSPEC jcursor {
        typedef boost::variant< json::array_t::size_type, string > index_t;
        typedef std::vector< index_t > stack_t;
    public:
        /// Create an empty jcursor representing the root of a JSON blob
        jcursor();
        /// Allow a jcursor to be implicitly created from a wide char literal
        jcursor( wliteral n );
        /// Allow a jcursor to be implicitly created from a narrow char literal
        jcursor( nliteral n );
        explicit jcursor( int i );
        explicit jcursor( json::array_t::size_type i );
        explicit jcursor( const string &p );
        explicit jcursor( const json &j );

        jcursor &operator /= ( int i ) { return (*this) /= json::array_t::size_type( i ); }
        jcursor &operator /= ( json::array_t::size_type i );
        jcursor &operator /= ( nliteral n ) { return (*this) /= fostlib::string(n); }
        jcursor &operator /= ( wliteral n ) { return (*this) /= fostlib::string(n); }
        jcursor &operator /= ( const string &i );
        jcursor &operator /= ( const json &j );
        jcursor &operator /= ( const jcursor &jc );

        template< typename T >
        jcursor operator / ( const T &i ) const {
            return jcursor( *this ) /= i;
        }

        jcursor &enter();
        jcursor &enter( const string &i );
        jcursor &pop();

        jcursor &operator++();

        json &operator() ( json &j ) const;

        json &push_back( json &j, const json &v ) const;
        json &insert( json &j, const json &v ) const;
        json &replace( json &j, const json &v ) const;
        json &del_key( json &j ) const;

        template< typename V >
        json &push_back( json &j, const V &v ) const {
            return push_back( j, json( v ) );
        }
        template< typename V >
        json &insert( json &j, const V &v ) const {
            return insert( j, json( v ) );
        }
        template< typename V >
        json &replace( json &j, const V &v ) const {
            return replace( j, json( v ) );
        }

        bool operator == ( const jcursor &j ) const;
        bool operator !=( const jcursor &j ) const {
            return !( *this == j );
        }

        /*
            Allow this jcursor to look a bit like a container
        */
        typedef stack_t::const_iterator const_iterator;
        const_iterator begin() const { return m_position.begin(); }
        const_iterator end() const { return m_position.end(); }
        typedef stack_t::size_type size_type;
        size_type size() const { return m_position.size(); }
        typedef index_t value_type;
        value_type operator [] ( size_type i ) const { return m_position.at( i ); }

    private:
        stack_t m_position;
        friend class json;

        jcursor( stack_t::const_iterator b, stack_t::const_iterator e );
    };


    template< typename F >
    struct coercer< json, nullable< F > > {
        json coerce( const nullable< F > &f ) {
            if ( f.isnull() )
                return json();
            else
                return fostlib::coercer< json, F >().coerce( f.value() );
        }
    };
    template< typename T >
    struct coercer< nullable< T >, json > {
        nullable< T > coerce( const json &f ) {
            if ( f.isnull() )
                return null;
            else
                return coercer< T, json >().coerce( f );
        }
    };


    /** \brief Allow us to coerce from any integral type to JSON
    */
    template< typename F >
    struct coercer<
        fostlib::json, F,
        typename boost::enable_if< boost::is_integral< F > >::type
    > {
        fostlib::json coerce( F i ) {
            return fostlib::json( fostlib::coerce< int64_t >( i ) );
        }
    };

    /** \brief Allow us to coerce to any integral type from JSON
    */
    template< typename T >
    struct coercer<
        T, fostlib::json,
        typename boost::enable_if< boost::is_integral< T > >::type
    > {
        T coerce( const fostlib::json &j ) {
            try {
                return fostlib::coerce< T >(
                    fostlib::coerce< int64_t >(
                        j.get< fostlib::json::atom_t >().value()
                    )
                );
            } catch ( fostlib::exceptions::exception &e ) {
                e.info() << L"Trying to cast from JSON to an integral type\n" <<
                    "JSON: " << fostlib::json::unparse(j, true) << std::endl;
                throw;
            }
        }
    };


    /// Allow us to do coercions to JSON as this regularises a lot of other code
    template<>
    struct coercer< json, json > {
        /// Just pass on the JSON we were given
        const json &coerce( const json &j ) {
            return j;
        }
    };

    template<>
    struct coercer< json, bool > {
        json coerce( bool b ) {
            return json( b );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< bool, json > {
        bool coerce( const json &f );
    };

    template<>
    struct coercer< json, double > {
        json coerce( double d ) {
            return json( d );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< double, json > {
        double coerce( const json &j );
    };

    template<>
    struct coercer< json, string > {
        json coerce( const string &s ) {
            return json( s );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, json > {
        string coerce( const json &f );
    };
    /// Allow us to convert narrow string literals to JSON
    template< std::size_t L >
    struct coercer< json, char[ L ] > {
        /// Perform the coercion
        json coerce(const char n[L]) {
            return json(fostlib::coerce<string>(n));
        }
    };
    /// Allow us to convert const narrow string literals to JSON
    template< std::size_t L >
    struct coercer< json, const char[ L ] > {
        /// Perform the coercion
        json coerce(const char n[L]) {
            return json(fostlib::coerce<string>(n));
        }
    };
    /// Allow us to convert const wide string literals to JSON
    template<>
    struct coercer< json, nliteral > {
        /// Perform the coercion
        json coerce(nliteral n) {
            return json(fostlib::coerce<string>(n));
        }
    };
    /// Allow us to convert wide string literals to JSON
    template< std::size_t L >
    struct coercer< json, wchar_t[ L ] > {
        /// Perform the coercion
        json coerce(const wchar_t w[L]) {
            return json(fostlib::coerce<string>(w));
        }
    };
    /// Allow us to convert const wide string literals to JSON
    template< std::size_t L >
    struct coercer< json, const wchar_t[ L ] > {
        /// Perform the coercion
        json coerce(const wchar_t w[L]) {
            return json(fostlib::coerce<string>(w));
        }
    };
    /// Allow us to convert const wide string literals to JSON
    template<>
    struct coercer< json, wliteral > {
        /// Perform the coercion
        json coerce(wliteral w) {
            return json(fostlib::coerce<string>(w));
        }
    };

    /// Allow us to convert from a variant to JSON
    template<>
    struct coercer< json, variant > {
        /// Perform the coercion
        json coerce(const variant &v) {
            return json(v);
        }
    };


    /// Allow us to push any JSON constructable object to the end of the root of the JSON blob
    template< typename V >
    inline fostlib::json &push_back( fostlib::json &j, const V &v ) {
        fostlib::jcursor().push_back(j, fostlib::json(v));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location with a blob
    template< typename C, typename V >
    inline fostlib::json &push_back( fostlib::json &j, const C &p, const V &v ) {
        fostlib::jcursor(p).push_back(j, fostlib::json(v));
        return j;
    }
    /// Allow us to push any JSON constructable object to the requested location with a blob
    template< typename C1, typename C2, typename V >
    inline fostlib::json &push_back( fostlib::json &j, const C1 &p1, const C2 &p2, const V &v ) {
        (fostlib::jcursor(p1)/p2).push_back(j, fostlib::json(v));
        return j;
    }


}



namespace std {


    /// When output unparse the JSON and pretty print it
    inline fostlib::ostream &operator <<( fostlib::ostream &o, const fostlib::json &s ) {
        return o << fostlib::json::unparse( s, true );
    }


}


#endif // FOST_JSON_HPP
