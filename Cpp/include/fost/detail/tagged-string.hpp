/*
    Copyright 2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TAGGED_STRING_HPP
#define FOST_TAGGED_STRING_HPP


#include <fost/exception/not_implemented.hpp>


namespace fostlib {


    template< typename Tag, typename Impl = string >
    class tagged_string {
        Impl m_string;
    public:
        typedef Tag tag_type;
        typedef Impl impl_type;
        typedef typename Impl::value_type value_type;
        typedef typename Impl::const_iterator const_iterator;

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
    struct coercer< string, ascii_string > {
        string coerce( const ascii_string &s ) {
            return fostlib::coerce< string >( s.underlying() );
        }
    };
    template<>
    struct coercer< std::string, ascii_string > {
        std::string coerce( const ascii_string &s );
    };
    template<>
    struct coercer< std::wstring, ascii_string > {
        std::wstring coerce( const ascii_string &s );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< ascii_string, string > {
        ascii_string coerce( const string &s );
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


namespace std {


    template< typename Tag, typename Impl >
    inline fostlib::ostream &operator <<( fostlib::ostream  &o, const fostlib::tagged_string< Tag, Impl > &s ) {
        return o << s.underlying();
    }
    // This is required so that tagged strings which use the non-native format can be output
    inline fostlib::ostream &operator <<( fostlib::ostream  &o, const fostlib::non_native_string &s ) {
        return o << fostlib::coerce< fostlib::string >( s );
    }

    template< typename Tag, typename Impl >
    struct less< fostlib::tagged_string< Tag, Impl > > : public binary_function< bool, fostlib::tagged_string< Tag, Impl >, fostlib::tagged_string< Tag, Impl > > {
        bool operator () ( const fostlib::tagged_string< Tag, Impl > &l, const fostlib::tagged_string< Tag, Impl > &r ) const {
            return less< Impl >()( l.underlying(), r.underlying() );
        }
    };


}


#endif // FOST_TAGGED_STRING_HPP
