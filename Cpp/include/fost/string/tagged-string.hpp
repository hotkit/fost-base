/*
	$Revision: $
	$Date: $
	Copyright (C) 2007, FSL Technologies Limited. Contact "http://fost.3.felspar.com".
*/


#ifndef FOST3_STRING_HPP
#define FOST3_STRING_HPP
#pragma once


#include "FOST.util.hpp"


#ifdef F3UTIL_EXPORTS
#define FOST_STRING_HPP_ARCHIVE L"$Archive: $"
#define FOST_STRING_HPP_REVISION L"$Revision: $"
#define FOST_STRING_HPP_DATE L"$Date: $"
#endif


namespace FSLib {


	namespace utf {


		struct F3UTIL_DECLSPEC utf8_tagged_traits {
			typedef std::string impl_type;
			typedef char base_value_type;
			typedef wchar_t non_base_value_type;
			static void to_impl( impl_type &, const non_base_value_type * );
		};
		struct F3UTIL_DECLSPEC utf16_tagged_traits {
			typedef std::wstring impl_type;
			typedef wchar_t base_value_type;
			typedef char non_base_value_type;
			static void to_impl( impl_type &, const non_base_value_type * );
		};
		struct F3UTIL_DECLSPEC utf32_tagged_traits {
			typedef FSLib::wstring impl_type;
			typedef wchar_t base_value_type;
			typedef char non_base_value_type;
			static void to_impl( impl_type &, const non_base_value_type * );
		};


	}


	template< typename Tag, typename From, typename Traits = typename From::tagged_traits >
	class encoded_string {
		typename Traits::impl_type m_string;
	public:
		typedef Tag tag_type;
		typedef From from_type;
		typedef Traits tagged_traits;
		typedef typename tagged_traits::base_value_type base_value_type;
		typedef typename tagged_traits::non_base_value_type non_base_value_type;
		typedef typename Traits::impl_type impl_type;
		typedef typename impl_type::const_iterator const_iterator;

		const typename tagged_traits::impl_type &underlying() const {
			return m_string;
		}

		encoded_string() {
		}
		encoded_string( const base_value_type *seq )
		: m_string( seq ) {
		}
		explicit encoded_string( const non_base_value_type *seq ) {
			tagged_traits::to_impl( m_string, seq );
		}
		encoded_string( const encoded_string &c )
		: m_string( c.m_string ) {
		}
		template< typename F >
		encoded_string( const encoded_string< Tag, F, Traits > &s )
		: m_string( s.underlying() ) {
		}
		explicit encoded_string( const from_type &s );

		const_iterator begin() const {
			return m_string.begin();
		}
		const_iterator end() const {
			return m_string.end();
		}

		encoded_string &operator =( const encoded_string &s ) {
			m_string = s.m_string;
			return *this;
		}
	};

	template< typename Tag, typename From >
	struct do_encode {
		static void encode( typename From::impl_type &into, typename From::const_iterator begin, typename From::const_iterator end );
	};

	template< typename Tag, typename From, typename Traits > inline
	encoded_string< Tag, From, Traits >::encoded_string( const From &s ) {
		do_encode< Tag, From >::encode( m_string, s.begin(), s.end() );
	}


	typedef encoded_string< void, utf::utf8_tagged_traits::impl_type, utf::utf8_tagged_traits > utf8_string;
	typedef encoded_string< void, utf::utf16_tagged_traits::impl_type, utf::utf16_tagged_traits > utf16_string;
	typedef encoded_string< void, utf::utf32_tagged_traits::impl_type, utf::utf32_tagged_traits > utf32_string;


}


#endif // FOST3_STRING_HPP
