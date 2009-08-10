/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-crypto.hpp"
#include <fost/crypto.hpp>
#include <fost/exception/not_implemented.hpp>


fostlib::digester::digester( fostlib::string (*)( const fostlib::string & ) ) {
    throw fostlib::exceptions::not_implemented( "fostlib::digester::digester( fostlib::string (*)( const fostlib::string & ) )" );
}

fostlib::digester &fostlib::digester::operator << ( const fostlib::string & ) {
    throw fostlib::exceptions::not_implemented( "fostlib::digester::operator << ( const fostlib::string & )" );
}

fostlib::digester &fostlib::digester::operator << ( const boost::filesystem::wpath & ) {
    throw fostlib::exceptions::not_implemented( "fostlib::digester::operator << ( const boost::filesystem::wpath & )" );
}
