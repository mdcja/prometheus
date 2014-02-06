/**
 * This file provides tests for the error library
 *
 * @brief Provides error tests
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides unit tests for error library. Unit tests are run using
 * the Check framework available at http://check.sourceforge.net/.
 */
#include <stdlib.h>
#include <check.h>
#include <string.h>

#include "error.h"

#define MAX_STRLEN 256

/**
 * Test get message
 *
 * This test gets the message given the error code.
 *
 * @post Normal program termination.
 */
START_TEST( test_error_get_message )
{
    char * message = NULL;

    /* test no error */
    error_code = ERROR_NO_ERROR;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "no error", MAX_STRLEN ) == 0 );

    /* test unknown */
    error_code = ERROR_UNKNOWN;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "unknown", MAX_STRLEN ) == 0 );

    /* test memory allocation failed */
    error_code = ERROR_MEMORY_ALLOCATION_FAILED;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "memory allocation failed", MAX_STRLEN ) == 0 );

    /* test null parameter */
    error_code = ERROR_NULL_PARAMETER;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "null parameter", MAX_STRLEN ) == 0 );

    /* test not found */
    error_code = ERROR_NOT_FOUND;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "not found", MAX_STRLEN ) == 0 );

    /* test invalid capacity */
    error_code = ERROR_INVALID_CAPACITY;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "invalid capacity", MAX_STRLEN ) == 0 );

    /* test not implemented */
    error_code = ERROR_NOT_IMPLEMENTED;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "not implemented", MAX_STRLEN ) == 0 );

    /* test overflow */
    error_code = ERROR_OVERFLOW;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "overflow", MAX_STRLEN ) == 0 );

    /* test underflow */
    error_code = ERROR_UNDERFLOW;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "underflow", MAX_STRLEN ) == 0 );

    /* test unknown error code < 0 */
    error_code = -10;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "unknown", MAX_STRLEN ) == 0 );

    /* test unknown error code > 10 */
    error_code = 10;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "unknown", MAX_STRLEN ) == 0 );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stderr.
 *
 * @post Normal program termination.
 */
START_TEST( test_error_print )
{
    int nchr = -1;

    /* test no error */
    error_code = ERROR_NO_ERROR;
    nchr = error_print( NULL );

    /* test unknown */
    error_code = ERROR_UNKNOWN;
    nchr = error_print( NULL );

    /* test memory allocation failed */
    error_code = ERROR_MEMORY_ALLOCATION_FAILED;
    nchr = error_print( NULL );

    /* test null parameter */
    error_code = ERROR_NULL_PARAMETER;
    nchr = error_print( NULL );

    /* test not found */
    error_code = ERROR_NOT_FOUND;
    nchr = error_print( NULL );

    /* test invalid capacity */
    error_code = ERROR_INVALID_CAPACITY;
    nchr = error_print( NULL );

    /* test not implemented */
    error_code = ERROR_NOT_IMPLEMENTED;
    nchr = error_print( NULL );

    /* test overflow */
    error_code = ERROR_OVERFLOW;
    nchr = error_print( NULL );

    /* test underflow */
    error_code = ERROR_UNDERFLOW;
    nchr = error_print( NULL );

    /* test unknown error code < 0 */
    error_code = -10;
    nchr = error_print( NULL );

    /* test unknown error code > 10 */
    error_code = 10;
    nchr = error_print( NULL );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * @post Normal program termination.
 */
START_TEST( test_error_print_message )
{
    int nchr = -1;
    FILE * fp = stdout;

    /* test no error */
    error_code = ERROR_NO_ERROR;
    nchr = error_print_message( fp, NULL );

    /* test unknown */
    error_code = ERROR_UNKNOWN;
    nchr = error_print_message( fp, NULL );

    /* test memory allocation failed */
    error_code = ERROR_MEMORY_ALLOCATION_FAILED;
    nchr = error_print_message( fp, NULL );

    /* test null parameter */
    error_code = ERROR_NULL_PARAMETER;
    nchr = error_print_message( fp, NULL );

    /* test not found */
    error_code = ERROR_NOT_FOUND;
    nchr = error_print_message( fp, NULL );

    /* test invalid capacity */
    error_code = ERROR_INVALID_CAPACITY;
    nchr = error_print_message( fp, NULL );

    /* test not implemented */
    error_code = ERROR_NOT_IMPLEMENTED;
    nchr = error_print_message( fp, NULL );

    /* test overflow */
    error_code = ERROR_OVERFLOW;
    nchr = error_print_message( fp, NULL );

    /* test underflow */
    error_code = ERROR_UNDERFLOW;
    nchr = error_print_message( fp, NULL );

    /* test unknown error code < 0 */
    error_code = -10;
    nchr = error_print_message( fp, NULL );

    /* test unknown error code > 10 */
    error_code = 10;
    nchr = error_print_message( fp, NULL );
}
END_TEST

/**
 * Error testing suite
 */
Suite * error_suite( void )
{
    Suite * s = suite_create( "Error" );;

    /* core tests cases */
    TCase * tc_core = tcase_create( "Core" );

    tcase_add_test( tc_core, test_error_get_message );
    tcase_add_test( tc_core, test_error_print );
    tcase_add_test( tc_core, test_error_print_message );

    suite_add_tcase( s, tc_core );

    return s;
}

/**
 * Error unit tests
 *
 * @post All unit tests specified in the testing suite are performed.
 */
int main( void )
{
    int number_failed = 0;
    Suite * s = error_suite();
    SRunner * sr = srunner_create( s );
    srunner_run_all( sr, CK_NORMAL );
    number_failed = srunner_ntests_failed( sr );
    srunner_free( sr );

    return ( number_failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
