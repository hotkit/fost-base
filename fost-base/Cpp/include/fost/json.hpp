/*
    Copyright 2007-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_JSON_HPP
#define FOST_JSON_HPP
#pragma once


#include <fost/variant.hpp>
#include <fost/coerce/ints.hpp>


namespace fostlib {


    class jcursor;

    class FOST_CORE_DECLSPEC json {
        friend class jcursor;
    public:
        typedef variant atom_t;
        typedef std::vector< boost::shared_ptr< json > > array_t;
        typedef string key_t;
        typedef std::map< key_t, boost::shared_ptr< json > > object_t;
        typedef boost::variant< atom_t, array_t, object_t > element_t;
        // We want to make sure that the underlying size types are the same
        BOOST_STATIC_ASSERT(
            sizeof( array_t::size_type ) == sizeof( object_t::size_type )
        );
    private:
        element_t m_element;
    public:

        json();
        template< typename T > explicit
        json( const T &t ) : m_element( atom_t( t ) ) {
        }
        explicit json( const atom_t &a ) : m_element( a ) {
        }
        json( const array_t &a ) : m_element( a ) {
        }
        json( const object_t &o ) : m_element( o ) {
        }
        explicit json( const element_t &e ) : m_element( e ) {
        }

        bool isnull() const;
        bool isatom() const;
        bool isarray() const;
        bool isobject() const;

        array_t::size_type size() const;

        bool has_key( array_t::size_type p ) const;
        bool has_key( wliteral n ) const { return has_key( fostlib::string(n) ); }
        bool has_key( nliteral n ) const { return has_key( fostlib::string(n) ); }
        bool has_key( const string &k ) const;
        bool has_key( const jcursor &p ) const;
        const json &operator [] ( wliteral n ) const { return (*this)[ fostlib::string(n) ]; }
        const json &operator [] ( nliteral n ) const { return (*this)[ fostlib::string(n) ]; }
        const json &operator [] ( const string &k ) const;
        const json &operator [] ( const jcursor &p ) const;
        // Check that the int promotion here is safe
        BOOST_STATIC_ASSERT( sizeof(int) <= sizeof(array_t::size_type) );
        const json &operator [] ( int p ) const { return (*this)[ array_t::size_type(p) ]; }
        const json &operator [] ( array_t::size_type p ) const;

        template< typename T >
        nullable< T > get() const {
            const atom_t *p = boost::get< atom_t >( &m_element );
            if ( p )
                return ( *p ).get< T >();
            else
                return null;
        }

        template< typename T >
        json &operator =( const T &t ) { m_element = atom_t( t ); return *this; }
        json &operator =( const array_t &a ) { m_element = a; return *this; }
        json &operator =( const object_t &o ) { m_element = o; return *this; }

        bool operator ==( const json &r ) const;
        bool operator !=( const json &r ) const { return !( *this == r ); }

        class FOST_CORE_DECLSPEC const_iterator {
            friend class json;
            const_iterator( const json &parent, array_t::const_iterator i );
            const_iterator( const json &parent, object_t::const_iterator i );
            typedef boost::variant<
                t_null,
                array_t::const_iterator,
                object_t::const_iterator
            > iterator_type;
        public:

            const_iterator();

            const json &operator * () const;
            json const *operator -> () const {
                return &**this;
            }
            const_iterator &operator ++ ();

            json key() const;

            bool operator ==( const_iterator r ) const;
            bool operator !=( const_iterator r ) const {
                return !( *this == r );
            }
        private:
            iterator_type m_iterator;
            const json *m_parent;
        };
        const_iterator begin() const;
        const_iterator end() const;

        template< typename T >
        typename T::result_type apply_visitor( T &t ) {
            return boost::apply_visitor( t, m_element );
        }
        template< typename T >
        typename T::result_type apply_visitor( T &t ) const {
            return boost::apply_visitor( t, m_element );
        }

        static json parse( const string & );
        static json parse( const string &, const json &def );
        static string unparse( const json &, bool pretty );
    };
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


}


namespace std {


    /// When output unparse the JSON and pretty print it
    inline fostlib::ostream &operator <<( fostlib::ostream &o, const fostlib::json &s ) {
        return o << fostlib::json::unparse( s, true );
    }


}


#endif // FOST_JSON_HPP
