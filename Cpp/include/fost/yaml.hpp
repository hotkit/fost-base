/**
    Copyright 2010-2018, Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#ifndef FOST_YAML_HPP
#define FOST_YAML_HPP
#pragma once


#include <fost/config.hpp>
#include <fost/nullable-core.hpp>


namespace fostlib {


    namespace yaml {


        /// A simple YAML record for output only
        class FOST_CORE_DECLSPEC record {
            typedef std::list< std::pair< std::size_t, string > > collection_type;
            collection_type content;
            public:
                /// Add a field without a value
                record &add( const string &key, t_null = null );
                /// Add a field with a string value
                record &add( const string &key, const string &value );
                /// Add a field of any type coerceable to a string
                template< typename V >
                record &add( const string &key, const V &value ) {
                    return add(key, coerce< string >( value ));
                }
                /// Add a field of any nullable type
                template< typename V >
                record &add( const string &key, const nullable<V> &nv ) {
                    if ( not nv )
                        return add(key);
                    else
                        return add(key, nv.value());
                }
                /// Add a nested YAML record
                record &add( const string &key, const record &record );
        };


    }


}


#endif // FOST_YAML_HPP
