/*
    Copyright 1999-2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_HOST_HPP
#define FOST_HOST_HPP
#pragma once


#include <fost/cli>


namespace fostlib {


    typedef uint32_t ipv4address;


	class FOST_INET_DECLSPEC host {
	public:
		host();
		explicit host( const fostlib::string & );
		explicit host( ipv4address );
		host( unsigned char, unsigned char, unsigned char, unsigned char );

		ipv4address address() const;
		fostlib::string name() const;

		bool operator ==( const host & ) const;
		bool operator !=( const host & ) const;

	private:
		fostlib::string m_name;
		mutable ipv4address m_address;
	};

	inline fostlib::ostream  &operator <<( fostlib::ostream  &o, const host &h ) {
		return o << h.name();
	}


}


#endif // FOST_HOST_HPP

