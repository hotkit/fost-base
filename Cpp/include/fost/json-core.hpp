/*
    Copyright 2007-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_JSON_CORE_HPP
#define FOST_JSON_CORE_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/nullable-core.hpp>
#include <fost/array>
#include <fost/string.hpp>

#include <boost/variant.hpp>


namespace fostlib {


    class jcursor;
    class json;

    using json_array = std::vector<json>;
    using json_object = std::map<string, json>;


    class FOST_CORE_DECLSPEC json {
        friend class jcursor;
    public:
        using array_t = json_array;
        using array_p = std::shared_ptr<array_t>;
        using object_t = json_object;
        using object_p = std::shared_ptr<object_t>;
        using element_t = boost::variant<t_null, bool, int64_t, double, string,
            std::shared_ptr<array_t>, std::shared_ptr<object_t>>;

        // We want to make sure that the underlying size types are the same
        static_assert(sizeof(array_t::size_type) == sizeof(object_t::size_type),
            "The underlying size types must be the same size");

    private:
        element_t m_element;
    public:

        /// Default construct to null
        json()
        : m_element(null) {
        }
        explicit json(t_null)
        : m_element(null) {
        }
        explicit json(bool b)
        : m_element(b) {
        }
        template<typename I>
        explicit json(I i, std::enable_if_t<std::is_integral<I>::value, void*> = nullptr)
        : m_element(int64_t(i)) {
        }
        explicit json(double d)
        : m_element(d) {
        }
        explicit json(const char *s)
        : m_element(string(s)) {
        }
        explicit json(const wchar_t *s)
        : m_element(string(s)) {
        }
        explicit json(string s)
        : m_element(std::move(s)) {
        }
        json(const array_t &a)
        : m_element(std::make_shared<array_t>(a)) {
        }
        json(array_t &&a)
        : m_element(std::make_shared<array_t>(std::move(a))) {
        }
        json(const object_t &o)
        : m_element(std::make_shared<object_t>(o)) {
        }
        json(object_t &&o)
        : m_element(std::make_shared<object_t>(std::move(o))) {
        }
        explicit json(element_t e)
        : m_element(e) {
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
        template<typename T>
        nullable<T> get() const {
            const T *p = boost::get<T>(&m_element);
            if ( p )
                return *p;
            else
                return null;
        }
        /// Fetch a value or the default. The value is returned only if the type exactly matches
        template<typename T>
        T get(T t) const {
            return get<T>().value_or(std::move(t));
        }

        /// Assignment from a nullable value follows assignment rules
        template<typename T>
        json &operator = (const nullable<T> &t) {
            if ( t.isnull() ) {
                m_element = null;
            } else {
                (*this) =t.value();
            }
            return *this;
        }
        json &operator = (t_null) {
            m_element = null;
            return *this;
        }
        json &operator = (bool b) {
            m_element = b;
            return *this;
        }
        json &operator = (int64_t i) {
            m_element = i;
            return *this;
        }
        json &operator = (double d) {
            m_element = d;
            return *this;
        }
        json &operator = (const string &s) {
            m_element = s;
            return *this;
        }
        json &operator = (string &&s) {
            m_element = std::move(s);
            return *this;
        }
        json &operator = (const array_t &a) {
            m_element = std::make_shared<array_t>(a);
            return *this;
        }
        json &operator = (array_t &&a) {
            m_element = std::make_shared<array_t>(std::move(a));
            return *this;
        }
        json &operator = (object_t &&o) {
            m_element = std::make_shared<object_t>(std::move(o));
            return *this;
        }
        json &operator = (const object_t &o) {
            m_element = std::make_shared<object_t>(o);
            return *this;
        }

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
