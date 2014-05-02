/**
 * This file provides an implementation of math functions.
 *
 * \brief provides generic math functions.
 * \author Julian Martinez del Campo
 * \license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file implements some generic math functions.
 */
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "error.h"
#include "math.h"

/**
 * Generate Primes with the Sieve of Eratosthenes 
 *
 * The marking is reversed, i.e. marking true = 0, and false = 1.
 *
 * Pseudocode
 *
 * 1) create a list of consecutive integers from 2 through n
 * 2) initially, let p equal 2, the first prime number
 * 3) starting from p, enumerate its multiples by counting to n in
 * increments of p, and mark them in the list (2p, 3p, 4p, etc) the p
 * itself should not be marked 4) Find the first number greater than p in
 * the list that is not marked. If there was no such number stop,
 * otherwise, let p now equal this new number (which is the next prime),
 * and repeat from step 3
 *
 * \note possible optimizations include sieving only odd numbers, using a more
 * complex wheel, packing the sieve into bits to imporove locality,
 * multithreading, etc.
 *
 * \pre limit is greater than 2.
 * \param limit.
 * \param primes_count.
 * \return the sieve with primes.
 */
static int * sieve_eratosthenes( unsigned int limit, int * primes_count )
{
    int * sieve = NULL;
    int i = 0;
    int j = 0;
    int n = 0;

    /* check */
    if( !primes_count )
    {
        error_code = ERROR_NULL_PARAMETER;
        error_print( "sieve_eratosthenes" );

        return NULL;
    }

    /* initialize sieve */
    sieve = (int *)calloc( limit + 1, sizeof( int ) );
    assert( sieve );
    if( !sieve )
    {
        error_code = ERROR_MEMORY_ALLOCATION_FAILED;
        error_print( "sieve_eratosthenes" );

        return NULL;
    }
    
    *primes_count = limit;          /* init number of primes */
    n = (int)sqrt( (double)limit ); /* set sieve limit */

    /* begin sieving */
    sieve[ 0 ] = 1;
    sieve[ 1 ] = 1;
    for( i = 2; i <= n; ++i )
    {
        if( sieve[ i ] == 0 )
        {
            for( j = i * i; j <= limit; j += i )
            {
                sieve[ j ] = 1;
                *(primes_count)--;
            }
        }
    }

    return sieve;
}

/**
 * Generate a prime number
 *
 * This function generates a prime number that is at least
 *  of size limit
 *
 * \param limit the limit at which prime numbers are not generated after. Must
 *  be greater than 2.
 * \return a prime number of at least limit
 */
unsigned long generate_prime( unsigned int limit )
{
    int largest_prime = 0;
    int prime_count = 0;
    int * sieve = NULL;
    int i = 0;

    /* check */
    if( limit <= 2 )
    {
        error_code = ERROR_INVALID_CAPACITY;
        error_print( "generate_prime" );

        return 0;
    }

    /* generate sieve */
    sieve = sieve_eratosthenes( limit, &prime_count );

    /* no prime numbers */
    if( prime_count <= 0 )
    {
        return 0;
    }

    /* look for largest prime */
    for( i = limit; i > 2; --i )
    {
        if( sieve[ i ] == 0 )
        {
            largest_prime = i;
            break;
        }
    }

    /* destroy sieve */
    free( sieve );
    sieve = NULL;

    return (unsigned int)largest_prime;
}

/**
 * Generate a random number
 *
 * This function generates a random number between min and max.
 *
 * \pre random number generator has been seeded
 * \param min the minimum number
 * \param max the max number
 * \return a random number between min and max
 */
int random_number( int min, int max )
{
    static int is_seeded = 0;

    if( !is_seeded )
    {
        srand( time( NULL ) );
    }

    return (rand()  % ( max - min )) + min;
}
