/*
    Copyright 2009, Felspar Co Ltd. http://fost.3.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/

#include <fost/cli>
#include <fost/thread.hpp>
#include <fost/main.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/construct.hpp>
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
        A class for wrapping the threading logic
    */
    class prime_sequence {
        /*
            A class for handling a sequence of prime numbers
        */
        struct prime_impl {
            prime_impl( std::size_t length )
            : m_primes( length ) // Make the vector large enough
            , m_generator( boost::bind( next_prime, 2U ) ) // Wrap a closure around next_prime
            {
                // Fills in the prime number sequence
                std::generate( m_primes.begin(), m_primes.end(), m_generator );
                // Initialise sum. std::accumulate is a fold
                m_sum = std::accumulate( m_primes.begin(), m_primes.end(), 0 );
                // The rest of the methods are predicated on the sequence sum initially being prime
                if ( !is_prime( m_sum ) )
                    next();
            }

            // Return the current sum
            unsigned int sum() const { return m_sum; }
            // Drop the smallest prime from the sequence and put the next largest in
            void shift() {
                std::copy( ++m_primes.begin(), m_primes.end(), m_primes.begin() );
                (*m_primes.rbegin()) = m_generator();
                m_sum = std::accumulate( m_primes.begin(), m_primes.end(), 0 );
            }
            // Move to the next seq which has a prime sum
            unsigned int next() {
                do shift(); while ( !is_prime( m_sum ) );
                return m_sum;
            }
        private:
            // The sum of the current sequence
            unsigned int m_sum;
            // The sequence of prime numbers of the desired length
            std::vector< unsigned int > m_primes;
            // A nullary function which returns a prime number
            boost::function< unsigned int ( void ) > m_generator;
        };
        // The thread holding the prime sequence
        boost::shared_ptr< fostlib::in_process< prime_impl > > m_worker;
        // Futures for the current prime sum and the next prime sum
        fostlib::result< unsigned int > m_sum, m_next;
    public:
        prime_sequence( std::size_t l )
        : m_worker( new fostlib::in_process< prime_impl >( new prime_impl( l ) ) ) {
            // Ask for the first sum
            m_sum = m_worker->asynchronous< unsigned int >( boost::lambda::bind( &prime_impl::sum, boost::lambda::_1 ) );
            // And start work on the next one
            m_next = m_worker->asynchronous< unsigned int >( boost::lambda::bind( &prime_impl::next, boost::lambda::_1 ) );
        }
        prime_sequence &operator ++ () {
            // We need to ask for the one after this in order to reduce the chance of blocking
            m_sum = m_next;
            m_next = m_worker->asynchronous< unsigned int >( boost::lambda::bind( &prime_impl::next, boost::lambda::_1 ) );
            return *this;
        }
        unsigned int sum() const {
            return m_sum();
        }
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
