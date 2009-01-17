/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main.hpp>
#include <boost/bind.hpp>


namespace {
    bool is_prime( int v ) {
        if ( v < 4 )
            return true;
        else if ( v % 2 == 0 )
            return false;
        else {
            for ( int i = 3; i < v; i += 2 )
                if ( v % i == 0 )
                    return false;
            return true;
        }
    }

    int next_prime( int &base ) {
        while ( !is_prime( base++ ) )
            ;
        return base - 1;
    }
}

boost::function< int ( void ) > prime_generator() {
    return boost::bind( next_prime, 2 );
}

FSL_MAIN(
    L"primes",
    L"Primes\nCopyright (c) 2009 Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::function< int ( void ) > generator = prime_generator();
    for ( int i = 0; i < 453; ++i )
        out << generator() << L" ";
    out << std::endl;
    return 0;
}
