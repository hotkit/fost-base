/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/cli>
#include <fost/main.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <numeric>


bool is_prime( unsigned int v );


namespace {

    unsigned int next_prime( unsigned int &base ) {
        while ( !is_prime( base++ ) )
            ;
        return base - 1;
    }

    struct prime_sequence {
        prime_sequence( std::size_t length )
        : primes( length ), generator( boost::bind( next_prime, 2U ) ) {
            std::generate( primes.begin(), primes.end(), generator );
            sum( std::accumulate( primes.begin(), primes.end(), 0 ) );
            if ( !is_prime( sum() ) )
                ++(*this);
        }

        fostlib::accessors< unsigned int > sum;

        void shift() {
            std::copy( ++primes.begin(), primes.end(), primes.begin() );
            (*primes.rbegin()) = generator();
            sum( std::accumulate( primes.begin(), primes.end(), 0 ) );
        }
        prime_sequence &operator ++ () {
            do shift(); while ( !is_prime( sum() ) );
            return *this;
        }
    private:
        std::vector< unsigned int > primes;
        boost::function< unsigned int ( void ) > generator;
    };

}


FSL_MAIN(
    L"primes",
    L"Primes\nCopyright (c) 2009 Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    std::list< prime_sequence > seqs;
    for ( std::size_t c( 1 ); c != args.size(); ++c )
        seqs.push_back( prime_sequence( fostlib::coerce< int >( args[ c ].value() ) ) );
    std::size_t matches;
    do {
        matches = 0;
        for ( std::list< prime_sequence >::iterator p( seqs.begin() ), n( ++seqs.begin() ); n != seqs.end(); ++p, ++n )
            if ( p->sum() < n->sum() )
                ++(*p);
            else if ( n->sum() < p->sum() )
                ++(*n);
            else
                ++matches;
    } while ( matches < seqs.size() - 1 );
    out << seqs.begin()->sum() << std::endl;
    return 0;
}
