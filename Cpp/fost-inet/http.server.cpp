/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-inet.hpp"
#include <fost/internet/http.server.hpp>
#include <fost/thread.hpp>


using namespace fostlib;


fostlib::http_server::http_server( const host &h, uint16_t p )
: binding( h ), port( p ), m_server( m_service, boost::asio::ip::tcp::endpoint( binding().address(), port() ) ) {
}
