/*
    Copyright 2001-2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <eh.h>
#include <fost/coerce/win.hpp>


fostlib::string fostlib::formatLastError() {
    LPVOID lpMsgBuf;
    if (!FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0,
            NULL ))
        return L"FormatMessage() problem - Cannot generate error message";
    fostlib::string ret( reinterpret_cast< LPCTSTR >( lpMsgBuf ) );
    LocalFree( lpMsgBuf );
    return ret;
}


class fostlib::exceptions::structure_information {
public:
    structure_information( _se_translator_function ofunc )
    : m_oldFunc( ofunc ) {
    }

public:
    _se_translator_function m_oldFunc;
};


namespace {


    void __cdecl structured( unsigned int, EXCEPTION_POINTERS *info ) {
        if ( fostlib::exceptions::structured::c_translate.value() ) {
            if ( info->ExceptionRecord->ExceptionCode != DBG_CONTROL_C )
                throw fostlib::exceptions::ctrl_break();
            else if ( info->ExceptionRecord->ExceptionCode != EXCEPTION_STACK_OVERFLOW &&
                    info->ExceptionRecord->ExceptionCode != EXCEPTION_BREAKPOINT )
                throw fostlib::exceptions::structured( *info );
        }
        throw;
    }


}


fostlib::exceptions::structured_handler::structured_handler()
: m_info( new structure_information( reinterpret_cast< _se_translator_function >( NULL ) ) ) {
    if ( fostlib::exceptions::structured::c_translate.value() )
        m_info->m_oldFunc = _set_se_translator( ::structured );
}


fostlib::exceptions::structured_handler::~structured_handler() {
    if ( m_info->m_oldFunc )
        _set_se_translator( m_info->m_oldFunc );
}


/*
    fost::exceptions::structured
*/


const fostlib::setting< bool > fostlib::exceptions::structured::c_translate( L"fost-base/Cpp/fost-core/exception.cpp", L"Exception", L"Translate structured", true, true );


fostlib::exceptions::structured::structured( const EXCEPTION_POINTERS &info )
: exception () {
    switch ( info.ExceptionRecord->ExceptionCode ) {
    case EXCEPTION_ACCESS_VIOLATION:
        m_info << L"The thread tried to read from or write to a virtual address for which it does not have the appropriate access." << std::endl;
        break;
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
        m_info << L"The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking." << std::endl;
        break;
    case EXCEPTION_BREAKPOINT:
        m_info << L"A breakpoint was encountered." << std::endl;
        break;
    case EXCEPTION_DATATYPE_MISALIGNMENT:
        m_info << L"The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on." << std::endl;
        break;
    case EXCEPTION_FLT_DENORMAL_OPERAND:
        m_info << L"One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value." << std::endl;
        break;
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
        m_info << L"The thread tried to divide a floating-point value by a floating-point divisor of zero." << std::endl;
        break;
    case EXCEPTION_FLT_INEXACT_RESULT:
        m_info << L"The result of a floating-point operation cannot be represented exactly as a decimal fraction." << std::endl;
        break;
    case EXCEPTION_FLT_INVALID_OPERATION:
        m_info << L"This exception represents any floating-point exception not included in this list." << std::endl;
        break;
    case EXCEPTION_FLT_OVERFLOW:
        m_info << L"The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type." << std::endl;
        break;
    case EXCEPTION_FLT_STACK_CHECK:
        m_info << L"The stack overflowed or underflowed as the result of a floating-point operation." << std::endl;
        break;
    case EXCEPTION_FLT_UNDERFLOW:
        m_info << L"The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type." << std::endl;
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        m_info << L"The thread tried to execute an invalid instruction." << std::endl;
        break;
    case EXCEPTION_IN_PAGE_ERROR:
        m_info << L"The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network." << std::endl;
        break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
        m_info << L"The thread tried to divide an integer value by an integer divisor of zero." << std::endl;
        break;
    case EXCEPTION_INT_OVERFLOW:
        m_info << L"The result of an integer operation caused a carry out of the most significant bit of the result." << std::endl;
        break;
    case EXCEPTION_INVALID_DISPOSITION:
        m_info << L"An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception." << std::endl;
        break;
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
        m_info << L"The thread tried to continue execution after a noncontinuable exception occurred." << std::endl;
        break;
    case EXCEPTION_PRIV_INSTRUCTION:
        m_info << L"The thread tried to execute an instruction whose operation is not allowed in the current machine mode." << std::endl;
        break;
    case EXCEPTION_SINGLE_STEP:
        m_info << L"A trace trap or other single-instruction mechanism signaled that one instruction has been executed." << std::endl;
        break;
    default:
        m_info << L"Unknown type of Structured Exception" << std::endl;
    }
}


const wchar_t * const fostlib::exceptions::structured::message() const {
    return L"Windows Structured Exception";
}




/*
    fostlib::exceptions::com_error
*/


fostlib::exceptions::com_error::com_error( const string &message )
: exception( message ) {
}


fostlib::exceptions::com_error::com_error( const string &message, const string &information )
: exception( message ) {
    info() << trim( information ).value( string() ) << std::endl;
}


#ifdef FOST_HAVE_MFC
    namespace {
        void detail_message( const _com_error &c, std::wostream &o ) {
            o << L"Details:" << std::endl;

            o << L"  Description: ";
            if ( !c.Description() )
                o << L"Unknown COM error - No error message contained in the exception decription." << std::endl;
            else
                o << L"\'" <<c.Description() << L"\'" << std::endl;

            o << L"  Source: ";
            if ( !c.Source() )
                o << L"Unknown source - No source contained in the exception description." << std::endl;
            else
                o << L"\'"<< c.Source() << L"\'" << std::endl;

            o << L"  Error Message: ";
            if ( !c.ErrorMessage() )
                o << L"Unknown" << std::endl;
            else
                o << L"\'" << c.ErrorMessage() << L"\'" << std::endl;
        }
    }
    fostlib::exceptions::com_error::com_error( const _com_error &c )
    : exception() {
        detail_message(c, m_info);
    }
    fostlib::exceptions::com_error::com_error( const _com_error &c, const string &s )
    : exception() {
        m_info << s << std::endl;
        detail_message(c, m_info);
    }
#endif


const wchar_t * const fostlib::exceptions::com_error::message() const {
    return L"COM Error";
}



/*
    fostlib::com_hr
*/


fostlib::com_hr fostlib::hresult;


fostlib::com_hr::com_hr( HRESULT hr, size_t line, const char *file ) {
    try {
        check( hr );
    } catch ( fostlib::exceptions::exception &e ) {
        e.info() << fostlib::string( file ) << L":" << coerce< fostlib::string >( line ) << std::endl;
        throw;
    }
}


void fostlib::com_hr::doThrow( HRESULT hr ) const {
    throw fostlib::exceptions::com_error( L"COM call failed - bad HRESULT (" + format( hr ) + L")" );
}


namespace {
    fostlib::string format() {
        IErrorInfo *pEO = NULL;
        try {
            if(S_OK == GetErrorInfo(NULL, &pEO)){
                BSTR bstrDesc = NULL;
                pEO->GetDescription(&bstrDesc);
                fostlib::string result = bstrDesc;
                SysFreeString(bstrDesc);
                return result;
            } else
                return L"Error info not found via GetErrorInfo";
        } catch ( ... ) {
            if ( pEO )
                pEO->Release();
            throw;
        }
    }
}


fostlib::string fostlib::com_hr::format( HRESULT hr ) {
    return fostlib::coerce< fostlib::string >( hr ) + L"\n" + ::format();
}


fostlib::string fostlib::com_hr::format( IUnknown * punk ) {
    ISupportErrorInfo *pSEI = NULL;
    HRESULT hr = punk->QueryInterface(IID_ISupportErrorInfo, reinterpret_cast< void ** >(&pSEI));
    if ( SUCCEEDED( hr ) ) {
        fostlib::string result = format( pSEI );
        pSEI->Release();
        return result;
    } else
        return L"Could not find ISupportErrorInfo on supplied IUnknown";
}

