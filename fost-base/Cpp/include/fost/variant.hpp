/*
    Copyright 2007-2010, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#ifndef FOST_VARIANT_HPP
#define FOST_VARIANT_HPP
#pragma once


#include <fost/variant-core.hpp>
#include <fost/coerce/nullable.hpp>


namespace fostlib {


#ifdef FOST_USE_LONG
    template<> inline
    nullable< long > variant::get() const {
        return coerce< nullable< long > >( get< int64_t >() );
    }
#endif

    template<>
    struct FOST_CORE_DECLSPEC coercer< bool, variant > {
        bool coerce( const variant &f );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< int64_t, variant > {
        int64_t coerce( const variant &f );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< double, variant > {
        double coerce( const variant &f );
    };
    template<>
    struct FOST_CORE_DECLSPEC coercer< string, variant > {
        string coerce( const variant &f );
    };


}


namespace std {


    inline fostlib::ostream &operator <<( fostlib::ostream &o, const fostlib::variant &v ) {
        return o << fostlib::coerce< fostlib::string >( v );
    }


}


#endif // FOST_VARIANT_HPP
