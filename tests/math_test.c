/**
 * This file provides tests for the math library
 *
 * @brief Provides math tests
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides unit tests for math library. Unit tests are run using
 * the Check framework available at http://check.sourceforge.net/.
 */
#include <stdlib.h>
#include <check.h>
#include <string.h>

#include <stdio.h>

#include "math.h"

#define LIMIT 100

/**
 * Test generate prime
 */
START_TEST( test_generate_prime )
{
    int prime = 0;

    /* check primes for some arbitrary limits */
    prime = generate_prime( LIMIT );
    ck_assert( prime == 97 );

    prime = generate_prime( 20 );
    ck_assert( prime == 19 );

    prime = generate_prime( 70 );
    ck_assert( prime == 67 );
}
END_TEST

/**
 * Test random number
 */
START_TEST( test_random_number )
{
    int same = 0;
    int i = 0;
    int n = 0;
    int t = 0;

    for( t = 0, i = 0; i < LIMIT; ++i )
    {
        n = random_number( 0, LIMIT );
        ck_assert( n >= 0 && n <= LIMIT );

        if( t == n )
        {
            ++same;
        }

        t = n;
    }

    /* check probability */
    ck_assert( same < LIMIT );
}
END_TEST

/**
 * Math testing suite
 */
Suite * math_suite( void )
{
    Suite * s = suite_create( "Math" );;

    /* core tests cases */
    TCase * tc_core = tcase_create( "Core" );

    tcase_add_test( tc_core, test_generate_prime );
    tcase_add_test( tc_core, test_random_number );

    suite_add_tcase( s, tc_core );

    return s;
}

/**
 * Math unit tests
 *
 * @post All unit tests specified in the testing suite are performed.
 */
int main( void )
{
    int number_failed = 0;
    Suite * s = math_suite();
    SRunner * sr = srunner_create( s );

    srunner_run_all( sr, CK_NORMAL );
    number_failed = srunner_ntests_failed( sr );

    srunner_free( sr );

    return ( number_failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
