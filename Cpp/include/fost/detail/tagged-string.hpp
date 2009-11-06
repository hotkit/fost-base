/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TAGGED_STRING_HPP
#define FOST_TAGGED_STRING_HPP


#include <fost/exception/not_implemented.hpp>
#include <fost/detail/coerce.hpp>


namespace fostlib {


    template< typename Tag, typename Impl = string >
    class tagged_string {
        Impl m_string;
    public:
        typedef Tag tag_type;
        typedef Impl impl_type;
        typedef typename impl_type::const_iterator const_iterator;
        typedef typename impl_type::size_type size_type;
        typedef typename impl_type::value_type value_type;

        enum t_encoding {
            encoded, unencoded
        };

        const impl_type &underlying() const {
            return m_string;
        }

        tagged_string() {
        }
        tagged_string( const typename impl_type::value_type *s, t_encoding e = encoded )
        : m_string( s ) {
            switch ( e ) {
            case encoded:
                tag_type::check_encoded( m_string );
                break;
            case unencoded:
                tag_type::do_encode( s, m_string );
                break;
            }
        }
        tagged_string( const impl_type &s, t_encoding e = encoded )
        : m_string( s ) {
            switch ( e ) {
            case encoded:
                tag_type::check_encoded( m_string );
                break;
            case unencoded:
                tag_type::do_encode( s, m_string );
                break;
            }
        }
        tagged_string( const typename impl_type::value_type *s, const typename impl_type::value_type *f, t_encoding e = encoded )
        : m_string( s, f ) {
            switch ( e ) {
            case encoded:
                tag_type::check_encoded( m_string );
                break;
            case unencoded:
                throw exceptions::not_implemented("tagged_string( const typename impl_type::value_type *s, const typename impl_type::value_type *e, t_encoding e = encoded )");
            }
        }
        template< typename I >
        tagged_string( I s, I f, t_encoding e = encoded )
        : m_string( s, f ) {
            switch ( e ) {
            case encoded:
                tag_type::check_encoded( m_string );
                break;
            case unencoded:
                throw exceptions::not_implemented("tagged_string( I s, I f, t_encoding e = encoded )");
            }
        }

        bool empty() const {
            return m_string.empty();
        }
        void clear() {
            m_string.clear();
        }
        void reserve(std::size_t s) {
            m_string.reserve(s);
        }

        const_iterator begin() const {
            return m_string.begin();
        }
        const_iterator end() const {
            return m_string.end();
        }

        bool operator ==( const tagged_string &t ) const {
            return m_string == t.m_string;
        }
        bool operator !=( const tagged_string &t ) const {
            return m_string != t.m_string;
        }

        tagged_string &operator =( const tagged_string &s ) {
            m_string = s.m_string;
            return *this;
        }

        tagged_string operator +( const tagged_string &s ) const {
            return tagged_string( m_string + s.m_string );
        }
        tagged_string &operator += ( value_type c ) {
            m_string += c;
            return *this;
        }
        tagged_string &operator += ( const tagged_string &s ) {
            m_string += s.m_string;
            return *this;
        }
    };


    /// Describes checks for UTF8 strings
    struct FOST_CORE_DECLSPEC utf8_string_tag {
        static void do_encode( fostlib::nliteral from, std::string &into );
        static void do_encode( const std::string &from, std::string &into );
        static void check_encoded( const std::string &s );
    };
    /// A UTF8 string is a std::string internally
    typedef tagged_string< utf8_string_tag, std::string > utf8_string;

    /// Coerce from a fostlib::string to a fostlib::utf8_string
    template<>
    struct FOST_CORE_DECLSPEC coercer< utf8_string, string > {
        utf8_string coerce( const string & );
    };
    /// Coerce from a fostlib::utf8_string to a fostlib::string
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, utf8_string > {
        string coerce( const utf8_string & );
    };
    /// Build a fostlib::utf8_string from a std::vector< utf8 >
    template<>
    struct FOST_CORE_DECLSPEC coercer< utf8_string, std::vector< utf8 > > {
        utf8_string coerce( const std::vector< utf8 > &str );
    };
    /// Preserve the current code -- this typedef is deprecated
    typedef utf8_string utf8string;


    struct FOST_CORE_DECLSPEC ascii_string_tag {
        static void do_encode( fostlib::nliteral from, std::string &into );
        static void do_encode( const std::string &from, std::string &into );
        static void check_encoded( const std::string &s );
    };
    typedef tagged_string< ascii_string_tag, std::string > ascii_string;

    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_string, std::vector< ascii_string::value_type > > {
        ascii_string coerce( const std::vector< ascii_string::value_type > &v );
    };

    template<>
    struct FOST_CORE_DECLSPEC coercer< string, ascii_string > {
        string coerce( const ascii_string &s );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_string, string > {
        ascii_string coerce( const string &s );
    };
    template<>
    struct coercer< std::string, ascii_string > {
        const std::string &coerce( const ascii_string &s ) {
            return s.underlying();
        }
    };
    template<>
    struct coercer< ascii_string, std::string > {
        ascii_string coerce( const std::string &s ) {
            return ascii_string( s );
        }
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< std::wstring, ascii_string > {
        std::wstring coerce( const ascii_string &s );
    };


    template<>
    struct FOST_CORE_DECLSPEC coercer< fostlib::string, fostlib::non_native_string > {
        fostlib::string coerce( const fostlib::non_native_string &s );
    };


}


namespace std {


    template< typename Tag, typename Impl >
    inline fostlib::ostream &operator <<( fostlib::ostream  &o, const fostlib::tagged_string< Tag, Impl > &s ) {
        return o << s.underlying();
    }
    /// Allows tagged strings which are based on a non-native string type to be output
    inline fostlib::ostream &operator <<( fostlib::ostream  &o, const fostlib::non_native_string &s ) {
        for ( fostlib::non_native_string::const_iterator c( s.begin() ); c != s.end(); ++c )
            o << *c;
        return o;
    }

    template< typename Tag, typename Impl >
    struct less< fostlib::tagged_string< Tag, Impl > > : public binary_function< bool, fostlib::tagged_string< Tag, Impl >, fostlib::tagged_string< Tag, Impl > > {
        bool operator () ( const fostlib::tagged_string< Tag, Impl > &l, const fostlib::tagged_string< Tag, Impl > &r ) const {
            return less< Impl >()( l.underlying(), r.underlying() );
        }
    };


}


#endif // FOST_TAGGED_STRING_HPP
