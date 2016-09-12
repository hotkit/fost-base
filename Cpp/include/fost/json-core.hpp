/*
    Copyright 2007-2016, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_JSON_CORE_HPP
#define FOST_JSON_CORE_HPP
#pragma once


#include <fost/variant-core.hpp>
#include <fost/array>


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

        /// Default construct to null
        json()
        : m_element(atom_t()) {
        }
        template< typename T > explicit
        json( const T &t ) : m_element( atom_t( t ) ) {
        }
        /// Construct from a nullable atomic value
        template<typename T> explicit
        json(const nullable<T> &t)
        : json() {
            if ( !t.isnull() ) m_element = atom_t(t.value());
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

        /// Fetch a value of the specified atomic type
        template< typename T >
        nullable< T > get() const {
            const atom_t *p = boost::get< atom_t >( &m_element );
            if ( p )
                return ( *p ).get< T >();
            else
                return null;
        }
        /// Fetch a value or the default. The value is returned only if the type exactly matches
        template<typename T>
        T get(T t) const {
            return get<T>().value(t);
        }

        template< typename T >
        json &operator =( const T &t ) { m_element = atom_t( t ); return *this; }
        /// Assignment from a nullable atomic type
        template<typename T>
        json &operator = (const nullable<T> &t) {
            if ( t.isnull() ) m_element = atom_t();
            else m_element = atom_t(t.value());
            return *this;
        }
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

        /// Parse a JSON string returning the content. Throws on parse
        /// error
        static json parse(const string &);
        /// Parse a JSON string in a character buffer.
        static json parse(array_view<unsigned char>);
        /// Parse a JSON string returning the content. Returns def on
        /// parse error
        static json parse(const string &, const json &def);

        /// Stringify the JSON data structure into the provided string instance
        static void unparse(std::string &, const json &, bool pretty);
        /// Return a string representing the JSON data structure
        static inline string unparse(const json &j, bool pretty) {
            std::string res;
            res.reserve(2048);
            unparse(res, j, pretty);
            return res;
        }

        /// Stringify the string according to JSON rules into the provided buffer
        static void unparse(std::string &, const string &);
        /// Stringify the string according to JSON rules
        static string unparse(const string &s) {
            std::string res;
            res.reserve(s.native_length() + 20); // The 20 is totally arbitrary
            unparse(res, s);
            return res;
        }
    };


}


#endif // FOST_JSON_CORE_HPP
