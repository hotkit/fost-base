/*
    Copyright 2009-2018, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/workerpool.hpp>

#include <mutex>


using namespace fostlib;


struct fostlib::workerpool::implementation {
    std::mutex m_mutex;
    std::vector<boost::shared_ptr<worker>> m_available;
    std::size_t m_peak;
};


fostlib::workerpool::workerpool() : impl(new implementation) {
    impl->m_peak = 0;
}
fostlib::workerpool::~workerpool() { delete impl; }


boost::shared_ptr<worker> fostlib::workerpool::assign() {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    if (!impl->m_available.size())
        return boost::shared_ptr<worker>(new worker);
    else {
        boost::shared_ptr<worker> w = impl->m_available.back();
        impl->m_available.pop_back();
        return w;
    }
}
void fostlib::workerpool::replace(boost::shared_ptr<worker> w) {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    impl->m_available.push_back(w);
    impl->m_peak = std::max(impl->m_peak, impl->m_available.size());
}


std::size_t fostlib::workerpool::available() {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    return impl->m_available.size();
}
std::size_t fostlib::workerpool::peak_used() {
    std::lock_guard<std::mutex> lock(impl->m_mutex);
    return impl->m_peak;
}
