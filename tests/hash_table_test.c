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
    return fprintf( fp, "(%8p) %-8d", (void *)data, *(int *)data );
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
 * Hash an integer
 */
unsigned long int_hash( const void * key )
{
    unsigned long hash = *(int*)key;
    unsigned long high_order = 0;

    high_order = hash & 0xf8000000;
    hash = hash << 5;
    hash = hash ^ ( high_order >> 27 );
    hash = hash ^ 1; /* n */

    return hash;
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
    return fprintf( fp, "(%8p) %-20s", (void *)s, (const char*)s );
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
 * Hash a string
 *
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 */
unsigned long string_hash( const void * key )
{
    unsigned char * t = NULL;
    unsigned long hash = 2166136261;
    int i = 0;

    t = (unsigned char *)key;
    for( i = 0; i < MAX_STRLEN; ++i )
    {
        hash = ( hash * 16777619 ) ^ t[ i ];
    }

    return hash;
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
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
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
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );
    ck_assert( hash_table_capacity( ht ) == DEFAULT_CAPACITY );

    /* resize empty table */
    rc = hash_table_reserve( &ht, SECONDARY_CAPACITY );
    ck_assert( rc == 0 );

    /* resize table with items */
    /* insert */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );
    }
    ck_assert( hash_table_capacity( ht ) == SECONDARY_CAPACITY );

    /* resize */
    rc = hash_table_reserve( &ht, DEFAULT_CAPACITY );
    ck_assert( rc == 0 );
    ck_assert( hash_table_capacity( ht ) == DEFAULT_CAPACITY );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test insert
 */
START_TEST( test_insert )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* insert */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );

        ck_assert( hash_table_count( ht ) == i + 1 );
    }

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test lookup
 */
START_TEST( test_lookup )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int * temp = NULL;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* lookup null values */
    temp = hash_table_lookup( ht, &numbers[ 0 ] );
    ck_assert( temp == NULL );

    temp = hash_table_lookup( ht, &numbers[ 10 ] );
    ck_assert( temp == NULL );

    /* insert */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );

        ck_assert( hash_table_count( ht ) == i + 1 );
    }

    /* lookup */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        temp = hash_table_lookup( ht, &numbers[ i ] );

        ck_assert( temp != NULL );
        ck_assert( temp == &numbers[ i ] );
    }

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test remove
 */
START_TEST( test_remove )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    int * temp = NULL;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* remove null values */
    rc = hash_table_remove( &ht, &numbers[ 0 ] );
    ck_assert( rc == -1 );
    ck_assert( hash_table_count( ht ) == 0 );

    /* insert items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );

        ck_assert( hash_table_count( ht ) == i + 1 );
    }

    /* remove */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        temp = hash_table_lookup( ht, &numbers[ i ] );
        ck_assert( temp != NULL );

        rc = hash_table_remove( &ht, &numbers[ i ] );
        ck_assert( rc == 0 );

        if( i == DEFAULT_CAPACITY )
        {
            ck_assert( hash_table_count( ht ) == 0 );
        }
        else
        {
            ck_assert( hash_table_count( ht ) == DEFAULT_CAPACITY - (i + 1) );
        }

        temp = hash_table_lookup( ht, &numbers[ i ] );
        ck_assert( temp == NULL );
    }

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print
 */
START_TEST( test_print )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* print empty table */
    rc = hash_table_print( ht, stdout );
    ck_assert( rc > 0 );

    /* add items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* print table with items */
    rc = hash_table_print( ht, stdout );
    ck_assert( rc > 0 );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test load
 */
START_TEST( test_load )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};
    double load = 0.0;

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* load */
    load = hash_table_load( ht );
    ck_assert( load == 0.0 );

    /* add items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );

        load = hash_table_load( ht );
        ck_assert( load == (double)hash_table_count( ht ) / hash_table_capacity( ht ) );
    }

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test capacity
 */
START_TEST( test_capacity )
{
    hash_table_t * ht = NULL;
    int rc = 0;

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    rc = hash_table_capacity( ht );
    ck_assert( hash_table_capacity( ht ) == DEFAULT_CAPACITY );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );

    /* create */
    ht = hash_table_create( NULL, NULL, SECONDARY_CAPACITY );
    ck_assert( ht != NULL );

    rc = hash_table_capacity( ht );
    ck_assert( hash_table_capacity( ht ) == SECONDARY_CAPACITY );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );

    /* create */
    ht = hash_table_create( NULL, NULL, TERCIARY_CAPACITY );
    ck_assert( ht != NULL );

    rc = hash_table_capacity( ht );
    ck_assert( hash_table_capacity( ht ) == TERCIARY_CAPACITY );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test count
 */
START_TEST( test_count )
{
    hash_table_t * ht = NULL;
    int rc = -1;
    int numbers[ DEFAULT_CAPACITY ] = {0};

    /* initialize test values */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* add items */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );

        /* test count */
        rc = hash_table_count( ht );
        ck_assert( rc == (i + 1) );
    }

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test memory
 */
START_TEST( test_memory )
{
    hash_table_t * ht = NULL;
    int rc = 0;

    /* create */
    ht = hash_table_create( NULL, NULL, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* memory */
    rc = hash_table_memory( ht );
    ck_assert( rc > 0 );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test pring string/integer pair
 */
START_TEST( test_print_string_int )
{
    hash_table_t * ht = NULL;
    attr_t kattr = {0};
    attr_t dattr = {0};
    int rc = -1;

    /* initialize test values */
    int numbers[ DEFAULT_CAPACITY ] = {0};
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* first 102 words of the poem "Ode to a Nightingale" by John Keats */
    char * words[ MAX_STRLEN ] = { "MY", "heart", "aches,", "and", "a",
    "drowsy", "numbness", "pains", "My", "sense,", "as", "though", "of",
    "hemlock", "I", "had", "drunk,", "Or", "emptied", "some", "dull", "opiate",
    "to", "the", "drains", "One", "minute", "past,", "and", "Lethe-wards",
    "had", "sunk:", "’T", "is", "not", "through", "envy", "of", "thy", "happy",
    "lot,", "But", "being", "too", "happy", "in", "thy", "happiness,—", "That",
    "thou,", "light-wingèd", "Dryad", "of", "the", "trees,", "In", "some",
    "melodious", "plot", "Of", "beechen", "green,", "and", "shadows",
    "numberless,", "Singest", "of", "Summer", "in", "full-throated", "ease.",
    "O", "for", "a", "draught", "of", "vintage,", "that", "hath", "been",
    "Cooled", "a", "long", "age", "in", "the", "deep", "delvèd", "earth,",
    "Tasting", "of", "Flora", "and", "the", "country-green,", "Dance,", "and",
    "Provençal", "song,", "and", "sunburnt", "mirth!" 
    };

    /* set key attributes */
    attr_init( &kattr );
    attr_set_copy( &kattr, string_copy );
    attr_set_free( &kattr, string_free );
    attr_set_compare( &kattr, string_compare );
    attr_set_hash( &kattr, string_hash );
    attr_set_print( &kattr, string_print );

    /* set data attributes */
    attr_init( &dattr );
    attr_set_copy( &dattr, int_copy );
    attr_set_free( &dattr, int_free );
    attr_set_compare( &dattr, int_compare );
    attr_set_hash( &dattr, int_hash );
    attr_set_print( &dattr, int_print );

    /* create */
    ht = hash_table_create( &kattr, &dattr, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* insert */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, words[ i ], &numbers[ i ] );
        ck_assert( rc == 0 );
    }

    /* print */
    hash_table_print( ht, stdout );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
}
END_TEST

/**
 * Test print up integer/string pair
 */
START_TEST( test_print_int_string )
{
    hash_table_t * ht = NULL;
    attr_t kattr = {0};
    attr_t dattr = {0};
    int rc = -1;

    /* initialize test values */
    int numbers[ DEFAULT_CAPACITY ] = {0};
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        numbers[ i ] = i + 1;
    }

    /* first 102 words of the poem "Ode to a Nightingale" by John Keats */
    char * words[ MAX_STRLEN ] = { "MY", "heart", "aches,", "and", "a",
    "drowsy", "numbness", "pains", "My", "sense,", "as", "though", "of",
    "hemlock", "I", "had", "drunk,", "Or", "emptied", "some", "dull", "opiate",
    "to", "the", "drains", "One", "minute", "past,", "and", "Lethe-wards",
    "had", "sunk:", "’T", "is", "not", "through", "envy", "of", "thy", "happy",
    "lot,", "But", "being", "too", "happy", "in", "thy", "happiness,—", "That",
    "thou,", "light-wingèd", "Dryad", "of", "the", "trees,", "In", "some",
    "melodious", "plot", "Of", "beechen", "green,", "and", "shadows",
    "numberless,", "Singest", "of", "Summer", "in", "full-throated", "ease.",
    "O", "for", "a", "draught", "of", "vintage,", "that", "hath", "been",
    "Cooled", "a", "long", "age", "in", "the", "deep", "delvèd", "earth,",
    "Tasting", "of", "Flora", "and", "the", "country-green,", "Dance,", "and",
    "Provençal", "song,", "and", "sunburnt", "mirth!" 
    };

    /* set key attributes */
    attr_init( &kattr );
    attr_set_copy( &kattr, int_copy );
    attr_set_free( &kattr, int_free );
    attr_set_compare( &kattr, int_compare );
    attr_set_hash( &kattr, int_hash );
    attr_set_print( &kattr, int_print );

    /* set data attributes */
    attr_init( &dattr );
    attr_set_copy( &dattr, string_copy );
    attr_set_free( &dattr, string_free );
    attr_set_compare( &dattr, string_compare );
    attr_set_hash( &dattr, string_hash );
    attr_set_print( &dattr, string_print );

    /* create */
    ht = hash_table_create( &kattr, &dattr, DEFAULT_CAPACITY );
    ck_assert( ht != NULL );

    /* insert */
    for( int i = 0; i < DEFAULT_CAPACITY; ++i )
    {
        rc = hash_table_insert( &ht, &numbers[ i ], words[ i ]  );
        ck_assert( rc == 0 );
    }

    /* print */
    hash_table_print( ht, stdout );

    /* destroy */
    rc = hash_table_destroy( &ht );
    ck_assert( rc == 0 );
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
    tcase_add_test( tc_core, test_load );
    tcase_add_test( tc_core, test_lookup );

    /* data tests */
    tcase_add_test( tc_data, test_print_string_int );
    tcase_add_test( tc_data, test_print_int_string );

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
