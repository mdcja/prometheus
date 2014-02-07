/**
 * This file provides tests for the hash table ADT
 *
 * @brief Provides unit tests for a generic hash table type
 * @author Julian Martinez del Campo
 * @license GNU Lesser General Public License (LGPL) https://www.gnu.org/copyleft/lesser.html
 *
 * This file provides unit tests for the hash table ADT. Unit tests are run
 * using the Check framework available at http://check.sourceforge.net/.
 */
#include <stdlib.h>
#include <assert.h>
#include <check.h>

#include "error.h"
#include "attributes.h"
#include "hash_table.h"

/* Both capacities are prime */
#define DEFAULT_CAPACITY 101
#define SECONDARY_CAPACITY 199
#define TERCIARY_CAPACITY 53

#define MAX_STRLEN 256

/**
 * Compare two integers
 */
int int_compare( const void * data1, const void * data2 )
{
    if( *(int *)data1 == *(int *)data2 )
    {
        return 0;
    }
    else if( *(int *)data1 < *(int *)data2 )
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * Print an integer
 */
int int_print( const void * data, FILE * fp )
{
    return fprintf( fp, "%d", *(int *)data );
}

/**
 * Copy an integer
 */
void * int_copy( const void * data )
{
    int * new_num = NULL;

    new_num = (int *)calloc( 1, sizeof( int ) );
    if( new_num == NULL )
    {
        fprintf( stderr, "Error! could not allocate integer\n" );
        return NULL;
    }

    *new_num = *(int*)data;

    return new_num;
}

/**
 * Deallocate an integer
 */
void int_free( void * data )
{
    free( data );
}

/**
 * Compare a string
 */
int string_compare( const void * s1, const void * s2 )
{
    return strncmp( (const char *)s1, (const char *)s2, MAX_STRLEN );
}

/**
 * Print a string
 */
int string_print( const void * s, FILE * fp )
{
    return fprintf( fp, "(%p) %s", (void *)s, (const char*)s );
}

/**
 * Copy a string
 */
void * string_copy( const void * s )
{
    char * string = NULL;

    string = (char *)calloc( MAX_STRLEN, sizeof( char ) );
    if( string == NULL )
    {
        fprintf( stderr, "Error! could not allocate string\n" );
        return NULL;
    }
    strncpy( string, (const char *)s, MAX_STRLEN );

    return string;
}

/**
 * Free a string
 */
void string_free( void * s )
{
    free( s );
}

/**
 * Test creation and deletion
 */
START_TEST( test_create_and_destroy )
{
    attr_t kattr = {0};
    attr_t dattr = {0};
    hash_table_t * ht = NULL;
    int rc = -1;

    /* create hash table without attributes */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );

    /* create hash table with attributes */
    rc = attr_init( &kattr );
    ck_assert( rc == 0 );

    rc = attr_init( &dattr );
    ck_assert( rc == 0 );

    ht = hash_table_create( &kattr, &dattr, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test clear
 */
START_TEST( test_clear )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create hash table */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY / 2; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* test clear on hash table */
    rc = hash_table_clear( &ht );
    ck_assert( rc == 0 );
    ck_assert( hash_table_count( ht ) == 0 );

    /* destroy hash table */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test reserve
 */
START_TEST( test_reserve )
{
}
END_TEST

/**
 * Test insert
 */
START_TEST( test_insert )
{
}
END_TEST

/**
 * Test lookup
 */
START_TEST( test_lookup )
{
}
END_TEST

/**
 * Test remove
 */
START_TEST( test_remove )
{
}
END_TEST

/**
 * Test print
 */
START_TEST( test_print )
{
}
END_TEST

/**
 * Test load
 */
START_TEST( test_load )
{
}
END_TEST

/**
 * Test capacity
 */
START_TEST( test_capacity )
{
}
END_TEST

/**
 * Test count
 */
START_TEST( test_count )
{
}
END_TEST

/**
 * Test memory
 */
START_TEST( test_memory )
{
}
END_TEST

/**
 * Test inserting string/integer pair
 */
START_TEST( test_insert_string_int )
{
}
END_TEST

/**
 * Test looking up string/integer pair
 */
START_TEST( test_lookup_string_int )
{
}
END_TEST

/**
 * Test remove string/integer pair
 */
START_TEST( test_remove_string_int )
{
}
END_TEST

/**
 * Create a test suite
 */
Suite * hash_table_suite( void )
{
    Suite * s = suite_create( "Hash Table" );
    TCase * tc_core = tcase_create( "Core" );
    TCase * tc_data = tcase_create( "Data" );
    TCase * tc_error = tcase_create( "Error" );

    /* core test cases */
    tcase_add_test( tc_core, test_create_and_destroy );
    tcase_add_test( tc_core, test_insert );
    tcase_add_test( tc_core, test_remove );
    tcase_add_test( tc_core, test_clear );
    tcase_add_test( tc_core, test_memory );
    tcase_add_test( tc_core, test_capacity );
    tcase_add_test( tc_core, test_count );
    tcase_add_test( tc_core, test_reserve );
    tcase_add_test( tc_core, test_print );

    /* data tests */

    /* error test cases */
    
    /* add core tests to suite */
    suite_add_tcase( s, tc_core );
    suite_add_tcase( s, tc_data );
    suite_add_tcase( s, tc_error );

    return s;
}

/**
 * Run test suite
 */
int main( void )
{
    int number_failed = 0;
    Suite * s = hash_table_suite();
    SRunner * sr = srunner_create( s );

    srunner_run_all( sr, CK_NORMAL );
    number_failed = srunner_ntests_failed( sr );

    srunner_free( sr );
    
    if( number_failed == 0 )
    {
        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}
