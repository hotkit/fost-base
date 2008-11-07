/*
    Copyright 1997-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


struct fostlib::counter::counter_impl {
    counter_impl( int v )
    : m_count( v ) {
    }
    boost::mutex m_mutex;
    int m_count;
};


fostlib::counter::counter()
: m_impl( new counter_impl( 0 ) ) {
}

fostlib::counter::~counter() {
    delete m_impl;
}


int fostlib::counter::operator ++() {
    boost::mutex::scoped_lock lock( m_impl->m_mutex );
    return ++m_impl->m_count;
}


int fostlib::counter::value() const {
    boost::mutex::scoped_lock lock( m_impl->m_mutex );
    return m_impl->m_count;
}
