/**
    Copyright 2007-2018, Felspar Co Ltd. <http://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_JSON_HPP
#define FOST_JSON_HPP
#pragma once


#include <fost/json-core.hpp>
#include <fost/nullable.hpp>
#include <fost/coerce/ints.hpp>
#include <fost/coerce/nullable.hpp>


namespace fostlib {


    template<> inline
    json::json(const nullable<string> &s)
    : m_element() {
        if ( s ) m_element = std::make_shared<string>(s.value());
    }
    template<> inline
    json::json(nullable<string> &&s)
    : m_element() {
        if ( s ) m_element = std::make_shared<string>(std::move(s.value()));
    }

    template<> [[deprecated("Use f5::u8view instead")]] inline
    nullable<string> json::get() const {
        const string_p *p = std::get_if<string_p>(&m_element);
        if ( p ) return **p;
        else return null;
    }

    template<> inline
    nullable<json::object_t> json::get() const {
        const object_p *o = std::get_if<object_p>(&m_element);
        if ( o ) return **o;
        else return null;
    }

    template<> inline
    nullable<json::array_t> json::get() const {
        const array_p *a = std::get_if<array_p>(&m_element);
        if ( a ) return **a;
        else return null;
    }


    class FOST_CORE_DECLSPEC jcursor {
    public:
        using string_t = string;
        using number_t = json::array_t::size_type;
        using index_t = std::variant<number_t, string_t>;
        using stack_t = std::vector<index_t>;

        /// Create an empty jcursor representing the root of a JSON blob
        jcursor();
        /// Allow a jcursor to be implicitly created from a wide char literal
        jcursor(wliteral n);
        /// Allow a jcursor to be implicitly created from a narrow char literal
        jcursor(nliteral n);
        /// Copy & move constructor
        jcursor(const jcursor &j)
        : m_position(j.m_position) {
        }
        jcursor(jcursor &&j)
        : m_position(std::move(j.m_position)) {
        }
        /// Converting constructors
        explicit jcursor(int i);
        explicit jcursor(json::array_t::size_type i);
        explicit jcursor(f5::u8view);
        explicit jcursor(string &&);
        explicit jcursor(const json &j);

        /// Construct a jcursor from a string using the requested string as separator
        static jcursor split(const string &s, const string &separator);

        /// Variadic jcursor constructor
        template<typename A1, typename A2, typename... As>
        explicit jcursor(A1 &&a1, A2 &&a2, As && ...a)
        : jcursor(std::forward<A1>(a1)) {
            append(std::forward<A2>(a2), std::forward<As>(a)...);
        }

        /// Assignment
        jcursor &operator = (jcursor &&j) {
            m_position = std::move(j.m_position);
            return *this;
        }
        jcursor &operator = (const jcursor &j) {
            m_position = j.m_position;
            return *this;
        }
        template<typename A1>
        jcursor &operator = (A1 &&a1) {
            m_position.empty();
            append(std::forward<A1>(a1));
            return *this;
        }

        /// Append operators
        jcursor &operator /= (int i) { return (*this) /= json::array_t::size_type( i ); }
        jcursor &operator /= (json::array_t::size_type i);
        jcursor &operator /= (nliteral n) { return (*this) /= fostlib::string(n); }
        jcursor &operator /= (wliteral n) { return (*this) /= fostlib::string(n); }
        jcursor &operator /= (f5::u8view);
        jcursor &operator /= (const json &j);
        jcursor &operator /= (const jcursor &jc);

        template<typename T>
        jcursor operator / (const T &i) const & {
            return jcursor(*this) /= i;
        }
        template<typename T>
        jcursor &&operator / (const T &i) && {
            return std::move((*this) /= i);
        }

        jcursor &enter();
        jcursor &enter( const string &i );
        jcursor &pop();

        jcursor &operator++();

        /// Copy from the root of the existing `json` to produce a new `json`
        /// instance that can be mutated. Return the head item so it can be
        /// assigned to.
        json &operator() ( json &j ) const;

        json &push_back( json &j, const json &v ) const;
        json &insert( json &j, const json &v ) const;
        json &replace( json &j, const json &v ) const;
        json &set(json &j, const json &v) const;
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

        /// Construct a jcursor from part of a different one
        jcursor(stack_t s)
        : m_position(std::move(s)) {
        }
        jcursor(stack_t::const_iterator b, stack_t::const_iterator e);


        /// ### Parsing jcursors

        /// From a JSON pointer URL fragment or JSON string
        static jcursor parse_json_pointer_string(f5::u8view);
        static jcursor parse_json_pointer_fragment(f5::u8view);


    private:
        stack_t m_position;
        friend class json;

        template<typename A1>
        void append(A1 &&a1) {
            (*this) /= jcursor(std::forward<A1>(a1));
        }
        template<typename A1, typename... As>
        void append(A1 &&a1, As && ...a) {
            (*this) /= jcursor(std::forward<A1>(a1));
            append(std::forward<As>(a)...);
        }
    };


    /// Allow comparison of parts of a jcursor to a literal
    bool operator == (const jcursor::value_type &l, nliteral r);


    template< typename F >
    struct coercer< json, nullable< F > > {
        json coerce( const nullable< F > &f ) {
            if ( not f )
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
                return fostlib::coerce<T>(fostlib::coerce<int64_t>(j));
            } catch ( fostlib::exceptions::exception &e ) {
                jcursor("action").insert(e.data(),
                    json("Trying to cast from JSON to an integral type"));
                jcursor("json").insert(e.data(), j);
                throw;
            }
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<int64_t, json > {
        int64_t coerce(const json &j);
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
    /// Allow conversion of JSON into strings. Coercion to a f5::u8view only
    /// works where the JSON is a string of some sort. If the JSON may be
    /// a number (for example) and you still want to try to get a string
    /// then you need to coerce to a fostlib::string.
    template<>
    struct FOST_CORE_DECLSPEC coercer<f5::u8view, json> {
        f5::u8view coerce(const json &);
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer<string, json> {
        string coerce(const json &f);
    };
    /// Convert to a nullable u8view. If the contained type is not a
    /// string then this will return null rather than throw an error
    template<>
    struct FOST_CORE_DECLSPEC coercer<nullable<f5::u8view>, json> {
        nullable<f5::u8view> coerce(const json &);
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


    /// Coercion of jcursor elements to underlying types
    template<>
    struct coercer<nullable<string>, jcursor::value_type> {
        nullable<string> coerce(const jcursor::value_type &v) {
            const auto *inner = std::get_if<string>(&v);
            if ( inner ) return *inner;
            else return null;
        }
    };
    template<>
    struct coercer<nullable<json::array_t::size_type>, jcursor::value_type> {
        nullable<json::array_t::size_type> coerce(const jcursor::value_type &v) {
            const auto *inner = std::get_if<json::array_t::size_type>(&v);
            if ( inner ) return *inner;
            else return null;
        }
    };

    /// Allow coerceion from a std::vector of something coerceable
    /// to JSON
    template<typename V>
    struct coercer<json, std::vector<V>> {
        json coerce(const std::vector<V> &v) {
            json::array_t a;
            for ( const auto &d : v )
                a.push_back(fostlib::coerce<json>(d));
            return a;
        }
    };


}


namespace std {


    /// Print monostate values
    template<typename C>
    basic_ostream<C> &operator << (basic_ostream<C> &o, monostate) {
        return o << "** null **";
    }
    /// When output unparse the JSON and pretty print it
    inline fostlib::ostream &operator << (
            fostlib::ostream &o, const fostlib::json &s
    ) {
        return o << fostlib::json::unparse(s, true);
    }
    /// When output convert to JSON and print that
    inline fostlib::ostream &operator << (
            fostlib::ostream &o, const fostlib::jcursor &p
    ) {
        return o << fostlib::json::unparse(
            fostlib::coerce<fostlib::json>(p), false);
    }


}


#endif // FOST_JSON_HPP
