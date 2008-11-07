/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/exception/win.hpp>


fostlib::string fostlib::guid() {
    GUID g;
    if ( CoCreateGuid( &g ) == S_OK ) {
        wchar_t *guid;
        if ( UuidToString( &g, reinterpret_cast< unsigned short ** >( &guid ) ) == RPC_S_OK ) {
            fostlib::string val = guid;
            RpcStringFree( reinterpret_cast< unsigned short ** >( &guid ) );
            return val;
        } else {
            throw fostlib::exceptions::com_error( L"UuidToString failed whilst trying to create a GUID" );
        }
    } else {
        throw fostlib::exceptions::com_error( L"CoCreateGUID failed whilst trying to create a GUID" );;
    }
}

