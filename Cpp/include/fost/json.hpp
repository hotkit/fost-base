/*
    Copyright 2007-2016, Felspar Co Ltd. http://support.felspar.com/
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
        if ( a ) {
            return *a;
        } else {
            return null;
        }
    }
    template<> inline
    nullable< json::object_t > json::get() const {
        const object_t *o = boost::get< object_t >( &m_element );
        if ( o ) {
            return *o;
        } else {
            return null;
        }
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

        /// Construct a jcursor from a string using the requested char as separator
        static jcursor split(const string &s, const string &separator);

        /// Variadic jcursor constructor
        template<typename A1, typename... As>
        explicit jcursor(const A1 &a1, const As & ...a)
        : jcursor(a1) {
            append(a...);
        }

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

        template<typename A1>
        void append(const A1 &a1) {
            (*this) /= jcursor(a1);
        }
        template<typename A1, typename... As>
        void append(const A1 &a1, const As & ...a) {
            (*this) /= jcursor(a1);
            append(a...);
        }
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


    /// Allow us to coerce from any integral type to JSON
    template< typename F >
    struct coercer<
        fostlib::json, F,
        typename boost::enable_if< boost::is_integral< F > >::type
    > {
        fostlib::json coerce( F i ) {
            return fostlib::json( fostlib::coerce< int64_t >( i ) );
        }
    };

    /// Allow us to coerce to any integral type from JSON
    template< typename T >
    struct coercer<
        T, fostlib::json,
        typename boost::enable_if< boost::is_integral< T > >::type
    > {
        T coerce( const fostlib::json &j ) {
            try {
                return fostlib::coerce< T >(
                    fostlib::coerce< int64_t >(
                        j.get< fostlib::json::atom_t >().value()));
            } catch ( fostlib::exceptions::exception &e ) {
                jcursor("action").insert(e.data(),
                    json("Trying to cast from JSON to an integral type"));
                jcursor("json").insert(e.data(), j);
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

    /// Allow conversion of a double into JSON
    template<>
    struct coercer< json, double > {
        json coerce( double d ) {
            return json( d );
        }
    };
    /// Allow conversion of JSON to a double
    template<>
    struct FOST_CORE_DECLSPEC coercer< double, json > {
        double coerce( const json &j );
    };

    /// Allow conversion of strings into JSON
    template<>
    struct coercer< json, string > {
        json coerce( const string &s ) {
            return json( s );
        }
    };
    /// Allow conversion of JSON into strings
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
    /// Allow us to convert from an JSON object_t to JSON
    template<>
    struct coercer< json, json::object_t > {
        /// Perform the coercion
        json coerce(const json::object_t &o) {
            return json(o);
        }
    };
    /// Allow us to convert from an JSON array_t to JSON
    template<>
    struct coercer< json, json::array_t > {
        /// Perform the coercion
        json coerce(const json::array_t &a) {
            return json(a);
        }
    };


    /// Allow conversion from a jcursor to an JSON array
    template<>
    struct FOST_CORE_DECLSPEC coercer< json, jcursor > {
        json coerce( const jcursor & );
    };
    /// Allow us to turn a JSON array into a jcursor
    template<>
    struct FOST_CORE_DECLSPEC coercer< jcursor, json > {
        jcursor coerce( const json & );
    };


    /// Allow coerceion from a std::vector of something coerceable
    /// to JSON
    template<typename V>
    struct coercer<json, std::vector<V>> {
        json coerce(const std::vector<V> &v) {
            json::array_t a;
            for ( const auto &d : v )
                a.push_back(boost::make_shared<json>(fostlib::coerce<json>(d)));
            return a;
        }
    };


}


namespace std {


    /// When output unparse the JSON and pretty print it
    inline fostlib::ostream &operator <<(
            fostlib::ostream &o, const fostlib::json &s ) {
        return o << fostlib::json::unparse( s, true );
    }
    /// When output convert to JSON and print that
    inline fostlib::ostream &operator <<(
            fostlib::ostream &o, const fostlib::jcursor &p ) {
        return o << fostlib::json::unparse(
            fostlib::coerce<fostlib::json>(p), false );
    }


}


#endif // FOST_JSON_HPP
