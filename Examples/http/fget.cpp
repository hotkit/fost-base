/*
    Copyright 2008-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main.hpp>
#include <fost/inet>
#include <fost/internet/http.useragent.hpp>


using namespace fostlib;


FSL_MAIN(
    L"fget",
    L"Simple HTTP client\nCopyright (C) 2008-2009, Felspar Co. Ltd."
)( fostlib::ostream &o, fostlib::arguments &args ) {
    // The URL to be fetched (default to localhost)
    string location = args[ 1 ].value( L"http://localhost/" );
    o << location << std::endl;
    // Create a user agent and request the URL
    http::user_agent browser;
    std::auto_ptr< http::user_agent::response > response( browser.get( url( location ) ) );
    o << response->headers() << std::endl;
    // If the body is HTML then display it
    if ( response->content_type().substr( 0, 5 ) == L"text/" )
        o << response->items().size() << L" items" << std::endl;
    else
        o << L"Not text : " << response->content_type() << std::endl;
    return 0;
}
