/* Author: Julian Martinez del Campo
 *
 * This file provides unit tests for error library. 
 *
 * Unit tests are run using the Check framework available at
 * http://check.sourceforge.net/.
 *
 * If _EMBEDDED macro is defined then ignore all functions that use stdio.h.
 * Useful when programming embedded devices.
 */
#define _POSIX_C_SOURCE 200809L
#define STRING_LENGTH 256
#define NUM_ADDITIONAL_CHARS 14

#include <stdlib.h>
#include <string.h>
#include <check.h>

#include "error.h"

/* Get the number of digits in an integer
 */
int num_digits( int n )
{
    int i = -1;
    int ndigits = 0;

    /* zero takes up one space */
    if( n == 0 )
    {
        return ndigits + 1;
    }

    /* take into account minus sign */
    if( n < 0 )
    {
        ndigits++;
    }

    /* count numbers */
    for( i = n; i != 0; i = i / 10 )
    {
        ndigits++;
    }

    return ndigits;
}

/* Get the expected number of characters of the print function
 */
int get_expected_chars( error_t error, char * message )
{
    int nchar_digit = -1;
    int nchar_error = -1;
    int nchar_message = -1;
    int nchar_additional = -1;

    /* populate values */
    nchar_digit = num_digits( error );
    nchar_error = strnlen( error_get_message( error ), STRING_LENGTH );
    nchar_message = strnlen( message, STRING_LENGTH );
    nchar_additional = NUM_ADDITIONAL_CHARS;

    return nchar_digit + nchar_error + nchar_message + nchar_additional;
}

/* Test number of digits with a negative single digit number
 */
START_TEST( test_num_digits_negative1 )
{
    int ndigits = -1;

    ndigits = num_digits( -1 );
    ck_assert( ndigits == 2 );
}
END_TEST

/* Test number of digits with a negative double digit number
 */
START_TEST( test_num_digits_negative10 )
{
    int ndigits = -1;
    ndigits = num_digits( -10 );
    ck_assert( ndigits == 3 );
}
END_TEST

/*Test number of digits with a negative triple digit number
 */
START_TEST( test_num_digits_negative100 )
{
    int ndigits = -1;
    ndigits = num_digits( -100 );
    ck_assert( ndigits == 4 );
}
END_TEST

/* Test the number of digits with zero
 */
START_TEST( test_num_digits0 )
{
    int ndigits = -1;
    ndigits = num_digits( 0 );
    ck_assert( ndigits == 1 );
}
END_TEST

/* Test the number of digits with a single digit number
 */
START_TEST( test_num_digits1 )
{
    int ndigits = -1;
    ndigits = num_digits( 1 );
    ck_assert( ndigits == 1 );
}
END_TEST

/* Test the number of digits with a double digit number
 */
START_TEST( test_num_digits10 )
{
    int ndigits = -1;
    ndigits = num_digits( 10 );
    ck_assert( ndigits == 2 );
}
END_TEST

/* Test the numbe of digits with a triple digit number
 */
START_TEST( test_num_digits100 )
{
    int ndigits = -1;
    ndigits = num_digits( 100 );
    ck_assert( ndigits == 3 );
}
END_TEST

/**
 * Test the number of expected characters
 */
START_TEST( test_get_expected_chars )
{
    int nchar = -1;
}
END_TEST

/* This test gets the message given the error code is E_NO_ERROR.
 */
START_TEST( test_error_get_message_no_error )
{
    char * message = NULL;

    /* test no error */
    error_code = E_NO_ERROR;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "no error", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given an E_UNKNOWN error.
 */
START_TEST( test_error_get_message_unknown )
{
    char * message = NULL;

    /* test unknown */
    error_code = E_UNKNOWN;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "unknown", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is
 * E_MEMORY_ALLOCATION_FAILED.
 */
START_TEST( test_error_get_message_memory_allocation_failed )
{
    char * message = NULL;

    /* test memory allocation failed */
    error_code = E_MEMORY_ALLOCATION_FAILED;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "memory allocation failed", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is E_NULL_PARAMETER.
 */
START_TEST( test_error_get_message_null_parameter )
{
    char * message = NULL;

    /* test null parameter */
    error_code = E_NULL_PARAMETER;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "null parameter", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is E_NOT_FOUND.
 */
START_TEST( test_error_get_message_not_found )
{
    char * message = NULL;

    /* test not found */
    error_code = E_NOT_FOUND;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "not found", STRING_LENGTH ) == 0 );
}
END_TEST

/* Test get message. This test gets the message given the error code is
 * E_INVALID_CAPACITY.
 */
START_TEST( test_error_get_message_invalid_capacity )
{
    char * message = NULL;

    /* test invalid capacity */
    error_code = E_INVALID_CAPACITY;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "invalid capacity", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is E_NOT_IMPLEMENTED.
 */
START_TEST( test_error_get_message_not_implemented )
{
    char * message = NULL;

    /* test not implemented */
    error_code = E_NOT_IMPLEMENTED;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "not implemented", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is E_OVERFLOW.
 */
START_TEST( test_error_get_message_overflow )
{
    char * message = NULL;

    /* test overflow */
    error_code = E_OVERFLOW;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "overflow", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is E_UNDERFLOW.
 */
START_TEST( test_error_get_message_underflow )
{
    char * message = NULL;

    /* test underflow */
    error_code = E_UNDERFLOW;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "underflow", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is negative.
 *
 * \post Error less than 0 checked.
 */
START_TEST( test_error_get_message_lessthan )
{
    char * message = NULL;

    /* test unknown error code < 0 */
    error_code = -10;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "unknown", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test gets the message given the error code is undefined.
 */
START_TEST( test_error_get_message_greaterthan )
{
    char * message = NULL;

    /* test unknown error code > 10 */
    error_code = 10;
    message = error_get_message( error_code );
    ck_assert( strncmp( message, "unknown", STRING_LENGTH ) == 0 );
}
END_TEST

/* This test prints all error messages if error is E_NO_ERROR.
 */
START_TEST( test_error_print_no_error )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NO_ERROR;

    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    /* test no error */
    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_UNKNOWN.
 */
START_TEST( test_error_print_unknown )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_UNKNOWN;

    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_MEMORY_ALLOCATION_FAILED.
 */
START_TEST( test_error_print_memory_allocation_failed )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_MEMORY_ALLOCATION_FAILED;

    /* test memory allocation failed */
    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_NULL_PARAMETER.
 */
START_TEST( test_error_print_null_parameter )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NULL_PARAMETER;

    /* test null parameter */
    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_NOT_FOUND.
 */
START_TEST( test_error_print_not_found )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NOT_FOUND;

    /* test not found */
    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_INVALID_CAPACITY.
 */
START_TEST( test_error_print_invalid_capacity )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_INVALID_CAPACITY;

    /* test invalid capacity */
    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_NOT_IMPLEMENTED.
 */
START_TEST( test_error_print_not_implemented )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NOT_IMPLEMENTED;

    /* test not implemented */
    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_OVERFLOW.
 */
START_TEST( test_error_print_overflow )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_OVERFLOW;

    /* test overflow */
    expected_chr = get_expected_chars( error_code, message );
    nchr = error_print( message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is E_UNDERFLOW.
 */
START_TEST( test_error_print_underflow )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_UNDERFLOW;

    /* test underflow */
    nchr = error_print( message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is negative.
 */
START_TEST( test_error_print_lessthan )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = -10;

    /* test unknown error code < 0 */
    nchr = error_print( message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/* This test prints all error messages if error is undefined.
 */
START_TEST( test_error_print_greaterthan )
{
    int nchr = -1;
    int expected_chr = -1;
    char * message = "test";

    error_code = 10;

    /* test unknown error code > 10 */
    nchr = error_print( message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_NO_ERROR is checked.
 */
START_TEST( test_error_print_message_no_error )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NO_ERROR;

    /* test no error */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_UNKNOWN is checked.
 */
START_TEST( test_error_print_message_unknown )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_UNKNOWN;

    /* test unknown */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_MEMORY_ALLOCATION_FAILED is checked.
 */
START_TEST( test_error_print_message_memory_allocation_failed )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_MEMORY_ALLOCATION_FAILED;

    /* test memory allocation failed */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_NULL_PARAMETER is checked.
 */
START_TEST( test_error_print_message_null_parameter )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NULL_PARAMETER;

    /* test null parameter */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_NOT_FOUND is checked.
 */
START_TEST( test_error_print_message_not_found )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NOT_FOUND;

    /* test not found */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_INVALID_CAPACITY is checked.
 */
START_TEST( test_error_print_message_invalid_capacity )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_INVALID_CAPACITY;

    /* test invalid capacity */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_NOT_IMPLEMENTED is checked.
 */
START_TEST( test_error_print_message_not_implemented )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_NOT_IMPLEMENTED;

    /* test not implemented */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_OVERFLOW is checked.
 */
START_TEST( test_error_print_message_overflow )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_OVERFLOW;

    /* test overflow */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing E_UNDERFLOW is checked.
 */
START_TEST( test_error_print_message_underflow )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = E_UNDERFLOW;

    /* test underflow */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing error less than 0 is checked.
 */
START_TEST( test_error_print_message_lessthan )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = -10;

    /* test unknown error code < 0 */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Printing an error greather than 9 is checked.
 */
START_TEST( test_error_print_message_greaterthan )
{
    int nchr = -1;
    FILE * fp = stdout;
    int expected_chr = -1;
    char * message = "test";

    error_code = 10;

    /* test unknown error code > 10 */
    nchr = error_print_message( fp, message );
    expected_chr = get_expected_chars( error_code, message );

    ck_assert( nchr > 0 );
    ck_assert( nchr == expected_chr );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Null message is checked.
 */
START_TEST( error_test_error_print_null_parameter )
{
    int nchr = -1;

    error_code = 0; /* init error code */

    nchr = error_print( NULL );

    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( nchr == -1 );

}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Null fp is checked.
 */
START_TEST( error_test_error_print_message_null_fp )
{
    int nchr = -1;

    error_code = 0; /* init error code */

    nchr = error_print_message( NULL, "test" );

    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( nchr == -1 );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Null message is checked.
 */
START_TEST( error_test_error_print_message_null_message )
{
    int nchr = -1;

    error_code = 0; /* init error code */

    nchr = error_print_message( stderr, NULL );

    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( nchr == -1 );
}
END_TEST

/**
 * Test error print
 *
 * This test prints all error messages to stdout.
 *
 * \post Null parameters are checked.
 */
START_TEST( error_test_error_print_message_null_parameters )
{
    int nchr = -1;

    error_code = 0; /* init error code */

    nchr = error_print_message( NULL, NULL );

    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( nchr == -1 );
}
END_TEST

/**
 * Error testing suite
 */
Suite * error_suite( void )
{
    Suite * s = suite_create( "Error" );;

    TCase * tc_helper = tcase_create( "Helper" );

    tcase_add_test( tc_helper, test_num_digits_negative1 );
    tcase_add_test( tc_helper, test_num_digits_negative10 );
    tcase_add_test( tc_helper, test_num_digits_negative100 );
    tcase_add_test( tc_helper, test_num_digits0 );
    tcase_add_test( tc_helper, test_num_digits1 );
    tcase_add_test( tc_helper, test_num_digits10 );
    tcase_add_test( tc_helper, test_num_digits100 );

    suite_add_tcase( s, tc_helper );

    /* core tests cases */
    TCase * tc_core = tcase_create( "Core" );

    tcase_add_test( tc_core, test_error_get_message_no_error );
    tcase_add_test( tc_core, test_error_get_message_unknown );
    tcase_add_test( tc_core, test_error_get_message_memory_allocation_failed );
    tcase_add_test( tc_core, test_error_get_message_null_parameter );
    tcase_add_test( tc_core, test_error_get_message_not_found );
    tcase_add_test( tc_core, test_error_get_message_invalid_capacity );
    tcase_add_test( tc_core, test_error_get_message_not_implemented );
    tcase_add_test( tc_core, test_error_get_message_overflow );
    tcase_add_test( tc_core, test_error_get_message_underflow );
    tcase_add_test( tc_core, test_error_get_message_lessthan );
    tcase_add_test( tc_core, test_error_get_message_greaterthan );

    tcase_add_test( tc_core, test_error_print_no_error );
    tcase_add_test( tc_core, test_error_print_unknown );
    tcase_add_test( tc_core, test_error_print_memory_allocation_failed );
    tcase_add_test( tc_core, test_error_print_null_parameter );
    tcase_add_test( tc_core, test_error_print_not_found );
    tcase_add_test( tc_core, test_error_print_invalid_capacity );
    tcase_add_test( tc_core, test_error_print_not_implemented );
    tcase_add_test( tc_core, test_error_print_overflow );
    tcase_add_test( tc_core, test_error_print_underflow );
    tcase_add_test( tc_core, test_error_print_lessthan );
    tcase_add_test( tc_core, test_error_print_greaterthan );

    tcase_add_test( tc_core, test_error_print_message_no_error );
    tcase_add_test( tc_core, test_error_print_message_unknown );
    tcase_add_test( tc_core, test_error_print_message_memory_allocation_failed );
    tcase_add_test( tc_core, test_error_print_message_null_parameter );
    tcase_add_test( tc_core, test_error_print_message_not_found );
    tcase_add_test( tc_core, test_error_print_message_invalid_capacity );
    tcase_add_test( tc_core, test_error_print_message_not_implemented );
    tcase_add_test( tc_core, test_error_print_message_overflow );
    tcase_add_test( tc_core, test_error_print_message_underflow );
    tcase_add_test( tc_core, test_error_print_message_lessthan );
    tcase_add_test( tc_core, test_error_print_message_greaterthan );

    suite_add_tcase( s, tc_core );

    /* error tests cases */
    TCase * tc_error = tcase_create( "Error" );

    tcase_add_test( tc_error, error_test_error_print_null_parameter );
    tcase_add_test( tc_error, error_test_error_print_message_null_fp );
    tcase_add_test( tc_error, error_test_error_print_message_null_message );
    tcase_add_test( tc_error, error_test_error_print_message_null_parameters );

    suite_add_tcase( s, tc_error );

    return s;
}

/**
 * Error unit tests
 *
 * \post All unit tests specified in the testing suite are performed.
 */
int main( void )
{
    int number_failed = 0;
    Suite * s = NULL;
    SRunner * sr = NULL;

    s = error_suite();
    sr = srunner_create( s );
    srunner_run_all( sr, CK_VERBOSE );
    number_failed = srunner_ntests_failed( sr );
    srunner_free( sr );

    return ( number_failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
