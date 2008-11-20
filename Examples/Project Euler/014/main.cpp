/*
    Copyright 2008, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main.hpp>
#include <iostream>


using namespace fostlib;

typedef unsigned int int_type;

int_type next_n( int_type n ) {
    if ( n % 2 )
        return 3 * n + 1;
    else
        return n / 2;
}

int_type chain_length( int_type n ) {
    if ( n == 1 )
        return 1;
    typedef std::vector< std::size_t > vector_type;
    static std::map< std::size_t, vector_type > cache;
    const std::size_t divisor = 1000;
    vector_type &cache_line( cache[ n / divisor ] );
    if ( cache_line.empty() ) {
        //std::cout << "Empty cache line for " << ( n / divisor ) << std::endl;
        cache_line = vector_type( divisor );
    }
    vector_type::value_type &item = cache_line[ n % divisor ];
    if ( item == 0 )
        item = 1 + chain_length( next_n( n ) );
    return item;
}


FSL_MAIN(
    L"p014",
    L"Project Euler puzzle 14"
)( fostlib::ostream &, fostlib::arguments & ) {
    std::pair< int_type, std::size_t > longest( 0, 0 );
    for ( int_type n = 1; n != 1000000; ++n ) {
        std::size_t length = chain_length( n );
        if ( length > longest.second ) {
            longest = std::make_pair( n, length );
            std::cout << longest.first << " has length " << longest.second << std::endl;
        }
    }
    return 0;
}
