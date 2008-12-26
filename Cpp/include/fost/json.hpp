/*
    Copyright 2007-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_JSON_HPP
#define FOST_JSON_HPP
#pragma once


#include <fost/variant.hpp>
#include <fost/coerce.hpp>


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
        BOOST_STATIC_ASSERT( sizeof( array_t::size_type ) == sizeof( object_t::size_type ) );
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
        bool has_key( const string &k ) const;
        bool has_key( const jcursor &p ) const;
        const json &operator[]( array_t::size_type p ) const;
        const json &operator[]( const string &k ) const;
        const json &operator[]( const jcursor &p ) const;

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
            const_iterator &operator ++ ();

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


    class FOST_CORE_DECLSPEC jcursor {
        typedef boost::variant< json::array_t::size_type, string > index_t;
    public:
        jcursor();

        jcursor operator[]( json::array_t::size_type i ) const;
        jcursor operator[]( const string &i ) const;
        jcursor operator[]( const json &j ) const;

        jcursor &enter();
        jcursor &enter( const string &i );
        jcursor &pop();

        jcursor &operator++();

        json &operator() ( json &j ) const;

        json &push_back( json &j, const json &v ) const;
        json &insert( json &j, const json &v ) const;
        json &replace( json &j, const json &v ) const;

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

    private:
        typedef std::vector< index_t > stack_t;
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
    struct coercer< json, int > {
        json coerce( int i ) {
            return json( int64_t( i ) );
        }
    };
    template<>
    struct coercer< int, json > {
        int coerce( json j ) {
            return fostlib::coerce< int >( fostlib::coerce< int64_t >( j ) );
        }
    };

#ifdef FOST_USE_LONG
    template<>
    struct coercer< json, long > {
        json coerce( long l ) {
            return json( int64_t( l ) );
        }
    };
    template<>
    struct coercer< long, json > {
        long coerce( const json &j ) {
            return fostlib::coerce< long >( fostlib::coerce< int64_t >( j ) );
        }
    };
#endif

    template<>
    struct coercer< json, int64_t > {
        json coerce( int64_t i ) {
            return json( i );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< int64_t, json > {
        int64_t coerce( const json &f );
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


    inline ostream &operator <<( ostream &o, const json &s ) {
        return o << coerce< string >( s );
    }


}


#endif // FOST_JSON_HPP
