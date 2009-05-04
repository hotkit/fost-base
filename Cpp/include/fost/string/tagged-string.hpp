/*
    Copyright  2007-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_TAGGED_STRING_HPP
#define FOST_TAGGED_STRING_HPP


namespace fostlib {


    template< typename Tag, typename Impl = typename Tag::impl_type >
    class tagged_string {
        Impl m_string;
    public:
        typedef Tag tag_type;
        typedef Impl impl_type;
        typedef typename Impl::const_iterator const_iterator;

        const impl_type &underlying() const {
            return m_string;
        }

        tagged_string() {
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
