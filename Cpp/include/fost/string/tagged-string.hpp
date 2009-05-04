/*
    Copyright  2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
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
        typedef typename Impl::const_iterator const_iterator;

        enum t_encoding {
            encoded, unencoded
        };

        const impl_type &underlying() const {
            return m_string;
        }

        tagged_string() {
        }
        tagged_string( const typename impl_type::char_type *s, t_encoding e ) {
            switch ( e ) {
            case encoded:
                m_string = s;
                tag_type::check_encoded( m_string );
                break;
            case unencoded:
                tag_type::do_encode( s, m_string );
                break;
            }
        }
        tagged_string( const impl_type &s, t_encoding e ) {
            switch ( e ) {
            case encoded:
                m_string = s;
                tag_type::check_encoded( m_string );
                break;
            case unencoded:
                tag_type::do_encode( s, m_string );
                break;
            }
        }

        const_iterator begin() const {
            return m_string.begin();
        }
        const_iterator end() const {
            return m_string.end();
        }

        template< typename T >
        bool operator ==( const T &t ) {
            return m_string == t;
        }
        bool operator ==( const tagged_string &t ) {
            return m_string == t.m_string;
        }
        template< typename T >
        bool operator !=( const T &t ) {
            return !( *this == t );
        }

        tagged_string &operator =( const tagged_string &s ) {
            m_string = s.m_string;
            return *this;
        }
    };


}


#endif // FOST_TAGGED_STRING_HPP
