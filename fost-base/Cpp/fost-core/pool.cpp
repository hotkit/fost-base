/*
    Copyright 2009-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/workerpool.hpp>


using namespace fostlib;


struct fostlib::workerpool::implementation {
    boost::mutex m_mutex;
    std::vector< boost::shared_ptr< worker > > m_available;
    std::size_t m_peak;
};


fostlib::workerpool::workerpool()
: impl(new implementation) {
    impl->m_peak = 0;
}
fostlib::workerpool::~workerpool() {
    delete impl;
}


boost::shared_ptr< worker > fostlib::workerpool::assign() {
    boost::mutex::scoped_lock lock(impl->m_mutex);
    if ( !impl->m_available.size() )
        return boost::shared_ptr< worker >( new worker );
    else {
        boost::shared_ptr< worker > w = impl->m_available.back();
        impl->m_available.pop_back();
        return w;
    }
}
void fostlib::workerpool::replace( boost::shared_ptr< worker > w ) {
    boost::mutex::scoped_lock lock(impl->m_mutex);
    impl->m_available.push_back(w);
    impl->m_peak = std::max( impl->m_peak, impl->m_available.size() );
}


std::size_t fostlib::workerpool::available() {
    boost::mutex::scoped_lock lock(impl->m_mutex);
    return impl->m_available.size();
}
std::size_t fostlib::workerpool::peak_used() {
    boost::mutex::scoped_lock lock(impl->m_mutex);
    return impl->m_peak;
}
