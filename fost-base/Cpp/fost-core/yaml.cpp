/*
    Copyright 2010, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-core.hpp"
#include <fost/yaml.hpp>


namespace {
    template< typename C >
    void push_back( C &c, const fostlib::string &v, std::size_t l = 0 ) {
        c.push_back( std::make_pair( l, v ) );
    }
}

fostlib::yaml::record &fostlib::yaml::record::add(
    const fostlib::string &k, fostlib::t_null
) {
    push_back( content, k + ": ~" );
    return *this;
}

fostlib::yaml::record &fostlib::yaml::record::add(
    const fostlib::string &k, const fostlib::string &v
) {
    push_back( content, k + ": " + v );
    return *this;
}

fostlib::yaml::record &fostlib::yaml::record::add(
    const fostlib::string &k, const fostlib::yaml::record &v
) {
    if ( &v == this )
        throw fostlib::exceptions::not_implemented(
            "fostlib::yaml::record::add -- recursively adding a record to itself"
        );
    push_back(content, k + ": [");
    for ( collection_type::const_iterator i = v.content.begin(); i != v.content.end(); ++i )
        push_back(content, i->second, i->first + 1);
    push_back(content, "]");
    return *this;
}
