/*
    Copyright 2001-2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"


namespace {


    const fostlib::setting< fostlib::string > c_format( L"fost-core/exception.cpp", L"Exception", L"Format", L"None", true );


}


void fostlib::absorbException() throw () {
    // An exception is in the process of being thrown away.
    // We want to be very careful not to do anything that may throw again.
}


/*
    fostlib::exceptions::exception
*/


fostlib::exceptions::exception::exception( const exception &e ) throw ()
: m_info() {
    try {
        m_info << e.info().str();
    } catch ( ... ) {
        absorbException();
    }
}


fostlib::exceptions::exception::exception() throw ()
: m_info() {
}


fostlib::exceptions::exception::exception( const fostlib::string &m ) throw ()
: m_info() {
    try {
        m_info << m << std::endl;
    } catch ( ... ) {
        absorbException();
    }
}


#ifdef _MSC_VER
    #pragma warning ( push )
    #pragma warning ( disable : 4297 ) // function assumed not to throw an exception but does
#endif
fostlib::exceptions::exception::~exception() throw ()
try {
} catch ( ... ) {
    absorbException();
}
#ifdef _MSC_VER
    #pragma warning ( pop )
#endif


fostlib::exceptions::exception &fostlib::exceptions::exception::operator =( const fostlib::exceptions::exception &e ) {
    typedef std::exception t_base;
    t_base::operator =( e );
    m_info.clear();
    m_what_string.reset( NULL );
    m_info << e.info().str();
    return *this;
}


const fostlib::stringstream &fostlib::exceptions::exception::info() const {
    return m_info;
}


fostlib::stringstream &fostlib::exceptions::exception::info() {
    return m_info;
}


const char *fostlib::exceptions::exception::what() const throw () {
    try {
        fostlib::stringstream ss;
        ss << *this;
        utf8_string text = coerce< utf8_string >(string(ss.str()));
        if ( c_format.value() == L"HTML" )
            text = replaceAll(text, "\n", "<br>");
        const std::size_t underlying_length = text.underlying().length() + 1;
        m_what_string.reset(new char[ underlying_length ]);
        std::copy(
            text.underlying().c_str(),
            text.underlying().c_str() + underlying_length,
            m_what_string.get());
        return m_what_string.get();
    } catch ( ... ) {
        return "Unknown fostlib::exceptions::exception. "
            "Exception throw during generation of exception description";
    }
}


fostlib::ostream &fostlib::exceptions::exception::printOn(
    fostlib::ostream &o
) const {
    return o << string( message() )
        << '\n' << m_info.str() <<
        "\nData: " << data();
}


/*
    Individual exceptions
*/


fostlib::exceptions::ctrl_break::ctrl_break() {
}


#include <fost/exception/cast_fault.hpp>
fostlib::exceptions::cast_fault::cast_fault( const string &message ) throw ()
: exception( message ) {
}
const wchar_t * const fostlib::exceptions::cast_fault::message() const throw () {
    return L"The requested cast is not possible";
}


#include <fost/exception/external_process_failure.hpp>
fostlib::exceptions::external_process_failure::external_process_failure() throw ()
: exception() {
}
fostlib::exceptions::external_process_failure::external_process_failure( const string &command ) throw ()
: exception( command ) {
}
const wchar_t * const fostlib::exceptions::external_process_failure::message() const throw () {
    return L"External process exit code not zero";
}


#include <fost/exception/file_error.hpp>
fostlib::exceptions::file_error::file_error( const string &message, const string &filename ) throw ()
: exception( message ) {
    try {
        info() << L"Filename: " << filename << std::endl;
    } catch ( ... ) {
        absorbException();
    }
}
const wchar_t * const fostlib::exceptions::file_error::message() const throw () {
    return L"A File I/O error occured";
}


#include <fost/exception/json_error.hpp>
fostlib::exceptions::json_error::json_error( const string &message ) throw ()
: exception( message ) {
}
fostlib::exceptions::json_error::json_error( const string &message, const json &value ) throw ()
: exception( message ) {
    try {
        info() << L"Value: " << json::unparse( value, true );
    } catch ( ... ) {
        fostlib::absorbException();
    }
}
fostlib::wliteral const fostlib::exceptions::json_error::message() const throw () {
    return L"JSON error";
}


#include <fost/thread.hpp>
fostlib::exceptions::forwarded_exception::forwarded_exception( const fostlib::string &message ) throw ()
: exception( message ) {
}
const wchar_t * const fostlib::exceptions::forwarded_exception::message() const throw () {
	return L"An exception has been forwarded from another thread";
}


#include <fost/exception/missing_setting.hpp>
fostlib::exceptions::missing_setting::missing_setting( const string &section, const string &name ) throw ()
: exception() {
    try {
        m_info << L"Section: " << section << std::endl << L"Name: " << name << std::endl;
    } catch ( ... ) {
        fostlib::absorbException();
    }
}
const wchar_t * const fostlib::exceptions::missing_setting::message() const throw () {
    return L"Setting is missing";
}


#include <fost/exception/not_a_number.hpp>
fostlib::exceptions::not_a_number::not_a_number( const string &message ) throw ()
: exception( message ) {
}
fostlib::exceptions::not_a_number::not_a_number( const string &message, const string &value ) throw ()
: exception( message ) {
    m_info << L"Value: " << value << std::endl;
}
const wchar_t * const fostlib::exceptions::not_a_number::message() const throw () {
    return L"Not a number";
}


#include <fost/exception/not_implemented.hpp>
fostlib::exceptions::not_implemented::not_implemented() throw ()
: exception() {
}
fostlib::exceptions::not_implemented::not_implemented( const string &func ) throw ()
: exception( func ) {
}
fostlib::exceptions::not_implemented::not_implemented( const string &func, const string &mess ) throw ()
: exception( func ) {
    m_info << mess << std::endl;
}
fostlib::exceptions::not_implemented::not_implemented( const string &func, const string &mes, const string &extra ) throw ()
: exception( func ) {
    m_info << mes << std::endl;
    m_info << extra << std::endl;
}
fostlib::exceptions::not_implemented::not_implemented(
    const string &func, boost::system::error_code error
) throw () : exception( func ) {
    m_info << "Error: " << error << "\n";
}
fostlib::exceptions::not_implemented::not_implemented(
    const string &func, boost::system::error_code error, const string &mess
) throw () : exception( func ) {
    m_info << "Error: " << error << "\n";
    m_info << mess << std::endl;
}
const wchar_t * const fostlib::exceptions::not_implemented::message() const throw () {
    return L"Feature not implemented";
}


#include <fost/exception/not_null.hpp>
fostlib::exceptions::not_null::not_null() throw () {
}
fostlib::exceptions::not_null::not_null( const string &mes ) throw ()
: exception( mes ) {
}
fostlib::exceptions::not_null::not_null( const string &mes, const string &inf ) throw ()
: exception( mes ) {
    info() << inf << std::endl;
}
const wchar_t * const fostlib::exceptions::not_null::message() const throw () {
    return L"A value was found where none was expected";
}


#include <fost/exception/not_unique.hpp>
fostlib::exceptions::not_unique::not_unique( const string &message ) throw ()
: exception( message ) {
}
fostlib::exceptions::not_unique::not_unique( const string &error, const string &context, const string &alternative1, const string &alternative2 ) throw ()
: exception( error ) {
    info() << L"Context: " << context << L"\nAlternative 1: " << alternative1 << L"\nAlternative 2: " << alternative2 << std::endl;
}
const wchar_t * const fostlib::exceptions::not_unique::message() const throw () {
    return L"Uniqueness violation";
}


#include <fost/exception/null.hpp>
fostlib::exceptions::null::null() throw () {
}
fostlib::exceptions::null::null( const string &mes ) throw ()
: exception( mes ) {
}
fostlib::exceptions::null::null( const string &mes, const string &inf ) throw ()
: exception( mes ) {
    info() << inf << std::endl;
}
const wchar_t * const fostlib::exceptions::null::message() const throw () {
    return L"Attempt to de-reference null value";
}


#include <fost/exception/out_of_range.hpp>
fostlib::exceptions::out_of_range_string::out_of_range_string( const string &mn, const string &mx, const string &v ) throw () {
    m_info << L"Value: " << v << std::endl;
    m_info << L"Minimum: " << mn << std::endl;
    m_info << L"Maximum: " << mx << std::endl;
}
fostlib::exceptions::out_of_range_string::out_of_range_string( const string &mes, const string &mn, const string &mx, const string &v ) throw ()
: exception( mes ) {
    m_info << L"Value: " << v << std::endl;
    m_info << L"Minimum: " << mn << std::endl;
    m_info << L"Maximum: " << mx << std::endl;
}
const wchar_t * const fostlib::exceptions::out_of_range_string::message() const throw () {
    return L"Value outside of allowable range";
}


#include <fost/exception/overflow.hpp>
fostlib::exceptions::overflow< fostlib::string >::overflow( const string &message ) throw ()
: exception( message ) {
}
fostlib::exceptions::overflow< fostlib::string >::overflow( const string &message, const string &n, const string &d, const string &m ) throw ()
: exception( message ) {
    m_info << L"Attempting to calculate: " << n << L" * " << m << L" / " << d << std::endl;
}
fostlib::exceptions::overflow< fostlib::string >::overflow( const string &n, const string &d, const string &m ) throw ()
: exception() {
    m_info << L"Attempting to calculate: " << n << L" * " << m << L" / " << d << std::endl;
}
const wchar_t * const fostlib::exceptions::overflow< fostlib::string >::message() const throw () {
    return L"An Overflow occured";
}


#include <fost/exception/parse_error.hpp>
fostlib::exceptions::parse_error::parse_error( const string &message ) throw ()
: exception( message ) {
}
fostlib::exceptions::parse_error::parse_error( const string &message, const string &value ) throw ()
: exception( message ) {
    info() << L"Value: " << value << '\n';
}
const wchar_t * const fostlib::exceptions::parse_error::message() const throw () {
    return L"Parsing error";
}


#include <fost/exception/settings_fault.hpp>
fostlib::exceptions::settings_fault::settings_fault( const string &error, const string &domain, const string &section, const string &name, const string &value ) throw ()
: exception( error ) {
    m_info << L"Domain: " << domain << std::endl << L"Section: " << section << std::endl << L"Name: " << name << std::endl << L"Value: " << value << std::endl;
}
const wchar_t * const fostlib::exceptions::settings_fault::message() const throw () {
    return L"Internal Setting library fault";
}


#include <fost/exception/unexpected_eof.hpp>
fostlib::exceptions::unexpected_eof::unexpected_eof() throw ()
: exception() {
}
fostlib::exceptions::unexpected_eof::unexpected_eof( const string &msg ) throw ()
: exception( msg ) {
}
fostlib::exceptions::unexpected_eof::unexpected_eof( const string &msg, const string &f ) throw ()
: exception( msg ) {
    try {
        m_info << L"Filename: " << f << std::endl;
    } catch ( ... ) {
        fostlib::absorbException();
    }
}
const wchar_t * const fostlib::exceptions::unexpected_eof::message() const throw () {
    return L"Unexpected EOF";
}


#include <fost/exception/unexpected_nil.hpp>
fostlib::exceptions::unexpected_nil::unexpected_nil() throw ()
: exception() {
}
fostlib::exceptions::unexpected_nil::unexpected_nil( const string &e ) throw ()
: exception( e ) {
}
const wchar_t * const fostlib::exceptions::unexpected_nil::message() const throw () {
	return L"Unexpected zero when processing Unicode stream";
}


#include <fost/exception/unicode_encoding.hpp>
fostlib::exceptions::unicode_encoding::unicode_encoding( const string &e ) throw ()
: exception( e ) {
}
const wchar_t * const fostlib::exceptions::unicode_encoding::message() const throw () {
	return L"Unicode encoding and decoding";
}


#ifdef FOST_OS_WINDOWS
    #include "exception-win.cpp"
#else
    #include "exception-linux.cpp"
#endif
