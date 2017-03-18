/*
    Copyright 2009-2017, Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/

#include <fost/cli>
#include <fost/main.hpp>
#include <numeric>

bool is_prime( unsigned int v );


namespace {

    // A function for returning the next higher prime number
    // State is stored in the parameter passed to it
    unsigned int next_prime( unsigned int &base ) {
        while ( !is_prime( base++ ) )
            ;
        return base - 1;
    }

    /*
        A class for handling a sequence of prime numbers
    */
    struct prime_sequence {
        prime_sequence( std::size_t length ) // Make the vector large enough
        : primes( length ),
            generator([last = 2U]() mutable {return next_prime(last);}) // Wrap a closure around next_prime
        {
            std::generate( primes.begin(), primes.end(), generator );
            sum( std::accumulate( primes.begin(), primes.end(), 0 ) );
            if ( !is_prime( sum() ) )
                ++(*this);
        }

        // Store the current sum
        fostlib::accessors< unsigned int > sum;

        // Drop the smallest prime from the sequence and put the next largest in
        void shift() {
            std::copy( ++primes.begin(), primes.end(), primes.begin() );
            (*primes.rbegin()) = generator();
            sum( std::accumulate( primes.begin(), primes.end(), 0 ) );
        }
        // Move to the next seq which has a prime sum
        prime_sequence &operator ++ () {
            do shift(); while ( !is_prime( sum() ) );
            return *this;
        }
    private:
        // The sequence of prime numbers of the desired length
        std::vector< unsigned int > primes;
        // A nullary function which returns a prime number
        boost::function< unsigned int ( void ) > generator;
    };

}


FSL_MAIN(
    L"primes",
    L"Primes\nCopyright (C) 2009 Felspar Co. Ltd."
)( fostlib::ostream &out, fostlib::arguments &args ) {
    std::list< prime_sequence > seqs;
    for ( std::size_t c( 1 ); c != args.size(); ++c )
        seqs.push_back( prime_sequence( fostlib::coerce< int >( args[ c ].value() ) ) );
    if ( seqs.size() < 2 )
        throw fostlib::exceptions::out_of_range< std::size_t >(
            L"You must supply at least two prime sequence lengths",
            2, std::numeric_limits< std::size_t >::max(), seqs.size()
        );
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
