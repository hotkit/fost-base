/*
    Copyright 1997-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


struct fostlib::counter::counter_impl {
    counter_impl( LONG c )
    : m_counter( c ) {
    }

    volatile LONG m_counter;
};


fostlib::counter::counter()
: m_impl( new fostlib::counter::counter_impl( 0 ) ) {
}

fostlib::counter::~counter() {
    delete m_impl;
}


int fostlib::counter::value() const {
    return m_impl->m_counter;
}


int fostlib::counter::operator ++() {
    return InterlockedIncrement( &(m_impl->m_counter) );
}


int fostlib::counter::operator --() {
    return InterlockedDecrement( &(m_impl->m_counter) );
}

