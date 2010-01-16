/*
    Copyright 1999-2009, Felspar Co Ltd. http://fost.3.felspar.com/
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


fostlib::string fostlib::format_last_error() {
    return format_error(GetLastError());
}

fostlib::string fostlib::format_error( LONG error_number ) {
    LPWSTR buffer = NULL;
    DWORD result = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        0, error_number, 0,
        reinterpret_cast< LPWSTR >(&buffer), 0,
        NULL
    );
    fostlib::string error;
    if ( result )
        error = fostlib::string(buffer, buffer + result);
    else
        error = L"No detailed error message is available";
    LocalFree(buffer);
    return error;
}
