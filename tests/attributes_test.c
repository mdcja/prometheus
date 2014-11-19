/* Author: Julian Martinez del Campo
 *
 * This file provides unit tests for attributes. Unit tests are run using the
 * Check framework available at http://check.sourceforge.net/.
 */
#include <check.h>
#include <stdlib.h>

#include "attributes.h"
#include "error.h"

/* Dummy functions for testing attributes
 */
static int dummy_compare( const void * d1, const void * d2 )
{
    return 0;
}

static int dummy_print( const void * d, FILE * dfp )
{
    return 0;
}

static void * dummy_copy( const void * d )
{
    return NULL;
}

static void dummy_free( void * d )
{
    return;
}

static unsigned long dummy_hash( const void * d )
{
    return 0;
}

/* This test takes an empty attributes structure and initializes it to default
 * values.
 */
START_TEST( test_attributes_init )
{
    attr_t attributes = {0};
    int rc = -1;

    rc = attr_init( &attributes );  /* initialize */

    /* check */
    ck_assert( rc == 0 );
    ck_assert( attributes.copy != NULL );
    ck_assert( attributes.free != NULL );
    ck_assert( attributes.compare != NULL );
    ck_assert( attributes.print != NULL );
    ck_assert( attributes.hash != NULL );
}
END_TEST

/* This test takes an empty attributes structure and sets the compare function.
 */
START_TEST( test_attributes_set_compare )
{
    attr_t attributes = {0};
    int rc = -1;

    rc = attr_set_compare( &attributes, dummy_compare );

    /* check */
    ck_assert( rc == 0 );
    ck_assert( attributes.compare == dummy_compare );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.print == NULL );
    ck_assert( attributes.hash == NULL );
}
END_TEST

/* This test takes an empty attributes structure and sets the print function.
 */
START_TEST( test_attributes_set_print )
{
    attr_t attributes = {0};
    int rc = -1;

    rc = attr_set_print( &attributes, dummy_print );

    /* check */
    ck_assert( rc == 0 );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.print == dummy_print );
    ck_assert( attributes.hash == NULL );
}
END_TEST

/* This test takes an empty attributes structure and sets the copy function.
 */
START_TEST( test_attributes_set_copy )
{
    attr_t attributes = {0};
    int rc = -1;

    rc = attr_set_copy( &attributes, dummy_copy );

    /* check */
    ck_assert( rc == 0 );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.copy == dummy_copy );
    ck_assert( attributes.print == NULL );
    ck_assert( attributes.hash == NULL );
}
END_TEST

/* This test takes an empty attributes structure and sets the free function.
 */
START_TEST( test_attributes_set_free )
{
    attr_t attributes = {0};
    int rc = -1;

    rc = attr_set_free( &attributes, dummy_free );

    /* check */
    ck_assert( rc == 0 );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.free == dummy_free );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.print == NULL );
    ck_assert( attributes.hash == NULL );
}
END_TEST

/* This test takes an empty attributes structure and sets the hash function.
 */
START_TEST( test_attributes_set_hash )
{
    attr_t attributes = {0};
    int rc = -1;
    
    rc = attr_set_hash( &attributes, dummy_hash );

    /* check */
    ck_assert( rc == 0 );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.print == NULL );
    ck_assert( attributes.hash == dummy_hash );
}
END_TEST

/* This test tries to initialize attributes without passing in an attributes
 * structure.
 */
START_TEST( error_test_attributes_init )
{
    int rc = 0;

    error_code = 0; /* initialize the error code */

    rc = attr_init( NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
}
END_TEST

/* This test passes a null attributes structure to the set compare function.
 */
START_TEST( error_test_attributes_set_compare_null_attributes )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null attributes */
    rc = attr_set_compare( NULL, dummy_compare );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set compare function.
 */
START_TEST( error_test_attributes_set_compare_null_function )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null compare function */
    rc = attr_set_compare( &attributes, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set compare function.
 */
START_TEST( error_test_attributes_set_compare_null_parameters )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null paramters */
    rc = attr_set_compare( NULL, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set print function.
 */
START_TEST( error_test_attributes_set_print_null_attributes )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null attributes */
    rc = attr_set_print( NULL, dummy_print );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set print function.
 */
START_TEST( error_test_attributes_set_print_null_function )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null print function */
    rc = attr_set_print( &attributes, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set print function.
 */
START_TEST( error_test_attributes_set_print_null_parameters )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null paramters */
    rc = attr_set_print( NULL, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set copy function.
 */
START_TEST( error_test_attributes_set_copy_null_attributes )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null attributes */
    rc = attr_set_copy( NULL, dummy_copy );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set copy function.
 */
START_TEST( error_test_attributes_set_copy_null_function )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null copy function */
    rc = attr_set_copy( &attributes, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set copy function.
 */
START_TEST( error_test_attributes_set_copy_null_parameters )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null parameters */
    rc = attr_set_copy( NULL, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set free function.
 */
START_TEST( error_test_attributes_set_free_null_attributes )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null attributes */
    rc = attr_set_free( NULL, dummy_free );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set free function.
 */
START_TEST( error_test_attributes_set_free_null_function )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null free function */
    rc = attr_set_free( &attributes, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set free function.
 */
START_TEST( error_test_attributes_set_free_null_parameters )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null parameters */
    rc = attr_set_free( NULL, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set hash function.
 */
START_TEST( error_test_attributes_set_hash_null_attributes )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null attributes */
    rc = attr_set_hash( NULL, dummy_hash );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set hash function.
 */
START_TEST( error_test_attributes_set_hash_null_function )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null hash function */
    rc = attr_set_hash( &attributes, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* This test passes null parameters to the set hash function.
 */
START_TEST( error_test_attributes_set_hash_null_parameters )
{
    attr_t attributes = {0};
    int rc = 0;

    error_code = 0; /* initialize the error code */

    /* test null parameters */
    rc = attr_set_hash( NULL, NULL );

    /* check */
    ck_assert( rc == -1 );
    ck_assert( error_code == E_NULL_PARAMETER );
    ck_assert( attributes.copy == NULL );
    ck_assert( attributes.free == NULL );
    ck_assert( attributes.compare == NULL );
    ck_assert( attributes.hash == NULL );
    ck_assert( attributes.print == NULL );
}
END_TEST

/* Attributes testing suite. Defines all the tests to be run.
 */
Suite * attributes_suite( void )
{
    Suite * s = suite_create( "Attributes" );;

    /* core tests cases */
    TCase * tc_core = tcase_create( "Core" );

    tcase_add_test( tc_core, test_attributes_init );
    tcase_add_test( tc_core, test_attributes_set_compare );
    tcase_add_test( tc_core, test_attributes_set_print );
    tcase_add_test( tc_core, test_attributes_set_copy );
    tcase_add_test( tc_core, test_attributes_set_free );
    tcase_add_test( tc_core, test_attributes_set_hash );

    suite_add_tcase( s, tc_core );

    /* error test cases */
    TCase * tc_error = tcase_create( "Error" );

    tcase_add_test( tc_core, error_test_attributes_init );
    tcase_add_test( tc_core, error_test_attributes_set_compare_null_attributes );
    tcase_add_test( tc_core, error_test_attributes_set_compare_null_function );
    tcase_add_test( tc_core, error_test_attributes_set_compare_null_parameters );
    tcase_add_test( tc_core, error_test_attributes_set_print_null_attributes );
    tcase_add_test( tc_core, error_test_attributes_set_print_null_function );
    tcase_add_test( tc_core, error_test_attributes_set_print_null_parameters );
    tcase_add_test( tc_core, error_test_attributes_set_copy_null_attributes );
    tcase_add_test( tc_core, error_test_attributes_set_copy_null_function );
    tcase_add_test( tc_core, error_test_attributes_set_copy_null_parameters );
    tcase_add_test( tc_core, error_test_attributes_set_free_null_attributes );
    tcase_add_test( tc_core, error_test_attributes_set_free_null_function );
    tcase_add_test( tc_core, error_test_attributes_set_free_null_parameters );
    tcase_add_test( tc_core, error_test_attributes_set_hash_null_attributes );
    tcase_add_test( tc_core, error_test_attributes_set_hash_null_function );
    tcase_add_test( tc_core, error_test_attributes_set_hash_null_parameters );

    suite_add_tcase( s, tc_error );

    return s;
}

/* Attributes unit tests. Run all unit tests.
 */
int main( void )
{
    int number_failed = 0;
    Suite * s = NULL;
    SRunner * sr = NULL;

    s = attributes_suite();
    sr = srunner_create( s );
    srunner_run_all( sr, CK_VERBOSE );
    number_failed = srunner_ntests_failed( sr );
    srunner_free( sr );

    return ( number_failed == 0 ) ? EXIT_SUCCESS : EXIT_FAILURE;
}
